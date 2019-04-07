#pragma once
/**
 * @file AudioSource.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the audio source
 *
 * @version 0.1
 * @date 2019/04/06
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>
#include <boost/thread.hpp>
#include "DSFFFmpeg.h"
#include "DSFXAudio2.h"
#include "DSFLogging.h"
#include "Component.hpp"

class AudioSource 
	: public Component
{
private:
	std::string filename;
	/**
	 * @brief The FFmpeg Framework reference
	 *
	 * Can only play/decode one file at a time
	 *
	 */
	DSFFFmpeg ffmpeg;
	IXAudio2SourceVoice* sourceVoice;
	// The callback will be used by the source voice
	DSFVoiceCallback callback;
	boost::thread playbackThread;

	void PlaySync();
public:
	AudioSource(Object* owner);
	~AudioSource();

	void LoadAudioFile(const std::string& filename);

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	void Play();
	void Stop();
	void Pause();

	bool Loop;
	bool isPlaying;
	bool stopped;
	bool fileOpened;
};

inline void AudioSource::PlaySync()
{
	if (!fileOpened) return;

	// Start the source voice
	sourceVoice->Start();

	LOG_TRACE << "Starting audio file \"" << filename << "\" playback at 0x" << boost::this_thread::get_id();

	bool stoppedByStreamEnd = true;

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

		LOG_WARNING << "Audio playback thread interrupted!";

		stoppedByStreamEnd = false;
	}

	// Stop the source voice
	sourceVoice->Stop();

	LOG_TRACE << "Stopping audio playback";

	// Clear all queued buffers
	sourceVoice->FlushSourceBuffers();

	ffmpeg.Seek(0, !stoppedByStreamEnd);

	isPlaying = false;
	stopped = true;
}

inline AudioSource::AudioSource(Object* owner)
	: Component(owner)
{
	isPlaying = false;
	stopped = true;
	fileOpened = false;
	Loop = false;
	sourceVoice = nullptr;
}

inline AudioSource::~AudioSource()
{
	if (sourceVoice != nullptr)
	{
		// Stop the source voice
		sourceVoice->Stop();

		// Clear all queued buffers
		sourceVoice->FlushSourceBuffers();

		// Destroy source voice
		sourceVoice->DestroyVoice();
	}
}

inline void AudioSource::LoadAudioFile(const std::string& filename)
{
	if (fileOpened) return;
	this->filename = filename;
	// Open file with FFmpeg, and initialize codecs
	int res = ffmpeg.OpenFile(this->filename.c_str());
	fileOpened = res >= 0;

	if (!fileOpened) return;

	// Parameters to be used when creating source voice
	int channels;
	int sampleRate;
	int bytesPerSample;

	// Initialize the resampler, and get the parameters required
	ffmpeg.InitSoftwareResampler(&channels, &sampleRate, &bytesPerSample);

	// Create the source voice with the parameters
	FXAudio2->CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);
}

inline void AudioSource::Start()
{
	ffmpeg.Init();
}

inline void AudioSource::Update(float deltaTime, float totalTime)
{
}

inline void AudioSource::Play()
{
	if (isPlaying)
	{
		return;
	}
	else 
	{
		if (stopped)
		{
			// stopped
			playbackThread = boost::thread(&AudioSource::PlaySync, this);
		}
		else 
		{
			// paused
			// resume
			sourceVoice->Start();

		}
	}
	isPlaying = true;
	stopped = false;
}

inline void AudioSource::Stop()
{
	if (isPlaying || (!isPlaying && !stopped))
	{
		SetEvent(callback.bufferEvent);
		playbackThread.interrupt();
		playbackThread.join();
	}
}

inline void AudioSource::Pause()
{
	if (!isPlaying) return;
	if (stopped) return;

	sourceVoice->Stop();

	isPlaying = false;
	stopped = false;
}
