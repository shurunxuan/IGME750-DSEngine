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
	//xAudio2.CreateSourceVoice(&sourceVoice, nullptr);
	xAudio2.CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);
	//ffmpeg.PlayFile(xAudio2.GetMasteringVoice(), sourceVoice, &callback);
	ffmpeg.SetXAudio2SourceVoice(sourceVoice);
	sourceVoice->Start();
	LOG_TRACE << "Starting audio playback";
	while (true)
	{
		int i = ffmpeg.BufferEnd();
		WaitForSingleObject(callback.bufferEvent, INFINITE);
		if (i != 0)
			break;
	}
	LOG_TRACE << "Waiting for the stream to end";
	WaitForSingleObject(callback.streamEvent, INFINITE);
	sourceVoice->Stop();
	LOG_TRACE << "Stopping audio playback";
	sourceVoice->FlushSourceBuffers();
}
