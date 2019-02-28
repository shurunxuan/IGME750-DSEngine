#include "DSSAudio.h"
#include "DSFLogging.h"


DSSAudio::DSSAudio()
{
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

void DSSAudio::OpenAudioFile(const char* filename)
{
	ffmpeg.OpenFile(filename);
}

void DSSAudio::PlayAudioFile(const char* filename)
{
	LOG_TRACE << "Creating new thread for playing audio";
	/**
	 * @todo Potential memory leaking when exit the program before the thread
	 *		 that contains the logging system is terminated. Will fix this
	 *	     when implementing thread pool.
	 */
	playThread = boost::thread(&DSSAudio::PlayAudioFileThread, this, filename);
}

void DSSAudio::PlayAudioFileThread(const char* filename)
{
	ffmpeg.OpenFile(filename);
	IXAudio2SourceVoice* sourceVoice = nullptr;
	DSFVoiceCallback callback;
	int channels;
	int sampleRate;
	int bytesPerSample;
	ffmpeg.InitSoftwareResampler(&channels, &sampleRate, &bytesPerSample);
	xAudio2.CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);
	sourceVoice->Start();
	LOG_TRACE << "Starting audio playback";
	while (true)
	{
		int i = ffmpeg.SendBuffer(sourceVoice);
		WaitForSingleObject(callback.bufferEvent, INFINITE);
		if (i != 0)
			break;
	}
	LOG_TRACE << "Waiting for the stream to end";
	if (WaitForSingleObject(callback.streamEvent, 10000) == WAIT_TIMEOUT)
	{
		LOG_WARNING << "Timeout. Forcing the stream to end. Please check if the XAUDIO2_END_OF_STREAM flag is properly set.";
	}
	sourceVoice->Stop();
	LOG_TRACE << "Stopping audio playback";
	sourceVoice->FlushSourceBuffers();
}
