/**
 * @file DSFFFmpeg.h
 * @author Victor Shu
 * @brief This file declares the framework class for FFmpeg.
 * @version 0.1
 * @date 2019/02/26
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
};
#include <xaudio2.h>

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

class DSFVoiceCallback;

class DSENGINEFRAMEWORK_API DSFFFmpeg
{
public:
	DSFFFmpeg();
	~DSFFFmpeg();

	void Init();

	int OpenFile(const char* filename);

	int ReadFrame();

	void InitSoftwareResampler(int* channels, int* sampleRate, int* bytesPerSample);
	int ResampleFrame();

	int SendBuffer(IXAudio2SourceVoice* sourceVoice);

private:
	AVFormatContext* formatContext;
	AVStream* audioStream;
	AVCodec* codec;
	AVCodecContext* codecContext;
	AVFrame* frame;
	AVFrame* lastFrame;
	AVPacket packet;
	SwrContext* swr;
	BYTE* swrBuffer;
	int swrBufferLength;
	BYTE** buffer;
	int bufferCount;
	bool eof;
};
