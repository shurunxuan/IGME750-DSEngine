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
	xAudio2.CreateSourceVoice(&sourceVoice, &callback);
	ffmpeg.SetXAudio2SourceVoice(sourceVoice);
	ffmpeg.InitSoftwareResampler(xAudio2.GetMasteringVoiceChannel(), xAudio2.GetMasteringVoiceSampleRate());
	sourceVoice->Start();
	LOG_TRACE << "Starting audio playback";
	while (true)
	{
		int i = ffmpeg.BufferEnd();
		if (i == 0) 
			WaitForSingleObject(callback.event, INFINITE);
		else 
			break;
	}
	LOG_TRACE << "Stopping audio playback";
	sourceVoice->Stop();
}
