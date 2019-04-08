#include "DSSAudio.h"
#include "DSFLogging.h"
#include "DSEngineApp.h"
#include "AudioSource.hpp"
#include "AudioListener.hpp"

DSSAudio* SAudio = nullptr;

DSSAudio::DSSAudio()
{
	SAudio = this;
}


DSSAudio::~DSSAudio()
{
}

void DSSAudio::Init()
{
	xAudio2.Init();
	ffmpeg.Init();
	LOG_TRACE << "DS Engine Audio System Initialized!";
}

void DSSAudio::Update()
{
	std::list<AudioSource*> audioSourcesInScene;

	// Get All AudioSources
	audioSourcesInScene.merge(App->CurrentActiveScene()->mainCamera->GetComponents<AudioSource>());
	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		audioSourcesInScene.merge(object->GetComponents<AudioSource>());
	}

	AudioListener* audioListener = nullptr;
	audioListener = App->CurrentActiveScene()->mainCamera->GetComponent<AudioListener>();
	if (audioListener == nullptr)
	{
		for (Object* object : App->CurrentActiveScene()->allObjects)
		{
			AudioListener* listener = object->GetComponent<AudioListener>();
			if (listener == nullptr) continue;
			audioListener = listener;
			break;
		}
	}
	if (audioListener == nullptr)
	{
		LOG_ERROR << "No AudioListener in scene!";
		return;
	}
	for (AudioSource* source : audioSourcesInScene)
	{
		// Don't calculate if source is not 3D
		if (!source->Is3D) continue;
		if (!source->isPlaying) continue;
		xAudio2.AudioCalculate3D(audioListener, source);
	}
}

void DSSAudio::PlayAudioFileNonBlock(const char* filename, boost::thread& playbackThread)
{
	LOG_TRACE << "Creating new thread for playing audio";

	playbackThread = boost::thread(&DSSAudio::PlayAudioFile, this, filename);
}

void DSSAudio::PlayAudioFile(const char* filename)
{
	// Open file with FFmpeg, and initialize codecs
	ffmpeg.OpenFile(filename);

	// The source voice to be created
	IXAudio2SourceVoice* sourceVoice = nullptr;

	// The callback will be used by the source voice
	DSFVoiceCallback callback;

	// Parameters to be used when creating source voice
	int channels;
	int sampleRate;
	int bytesPerSample;

	// Initialize the resampler, and get the parameters required
	ffmpeg.InitSoftwareResampler(&channels, &sampleRate, &bytesPerSample);

	// Create the source voice with the parameters
	xAudio2.CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);

	// Start the source voice
	sourceVoice->Start();

	LOG_TRACE << "Starting audio playback";

	try
	{
		while (true)
		{
			// If the source voice is going to be starved, decode & send buffer with FFmpeg
			int i = ffmpeg.SendBuffer(sourceVoice);

			boost::this_thread::interruption_point();
			
			// Waiting for the source voice buffer to end
			WaitForSingleObject(callback.bufferEvent, INFINITE);

			// If FFmpeg hit the end of file, break the loop
			if (i != 0)
				break;
		}

		LOG_TRACE << "Waiting for the stream to end";

		// Waiting the source voice stream to end
		if (WaitForSingleObject(callback.streamEvent, 10000) == WAIT_TIMEOUT)
		{
			// If timeout, force the stream to end
			LOG_WARNING << "Timeout. Forcing the stream to end. Please check if the XAUDIO2_END_OF_STREAM flag is properly set.";
		}

	}
	catch (boost::thread_interrupted const& tie)
	{
		UNREFERENCED_PARAMETER(tie);

		LOG_WARNING << "Audio playback thread interrupted! Stopping & destroying source voice";

		// Destroy source voice
		sourceVoice->DestroyVoice();

		return;
	}

	// Stop the source voice
	sourceVoice->Stop();

	LOG_TRACE << "Stopping audio playback";

	// Clear all queued buffers
	sourceVoice->FlushSourceBuffers();

	// Destroy source voice
	sourceVoice->DestroyVoice();
}
