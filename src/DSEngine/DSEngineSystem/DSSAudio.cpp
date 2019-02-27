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
	ffmpeg.OpenFile(filename);
	IXAudio2SourceVoice* sourceVoice = nullptr;
	DSFVoiceCallback callback;
	xAudio2.CreateSourceVoice(&sourceVoice, &callback);
	ffmpeg.SetXAudio2SourceVoice(sourceVoice);
	ffmpeg.InitSoftwareResampler(xAudio2.GetMasteringVoiceChannel(), xAudio2.GetMasteringVoiceSampleRate());
	xAudio2.StartSourceVoice(sourceVoice);
	while (true)
	{
		int i = ffmpeg.BufferEnd();
		WaitForSingleObject(callback.event, INFINITE);
		if (i != 0) break;
	}
}
