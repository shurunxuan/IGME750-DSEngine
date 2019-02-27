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
#include "DSFXAudio2.h"

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



typedef void (*FFmpegCallback)(void*, unsigned char*, int);

class DSENGINEFRAMEWORK_API DSFFFmpeg
{
public:
	DSFFFmpeg();
	~DSFFFmpeg();

	void Init();

	int OpenFile(const char* filename);

	int ReadFrame();

	void InitSoftwareResampler(int channel, int sampleRate);
	int ResampleFrame();

	void SetXAudio2SourceVoice(IXAudio2SourceVoice* sourceVoice);

	int BufferEnd();

private:
	AVFormatContext* formatCtx;
	AVStream* audioStream;
	AVCodec* codec;
	AVCodecContext* codecCtx;
	AVFrame* frame;
	AVPacket packet;
	SwrContext* swr;
	BYTE* swr_buf;
	int swr_buf_len;
	IXAudio2SourceVoice* sourceVoice;
	BYTE** buf;
	int buf_cnt;
	bool eof;
};

class DSENGINEFRAMEWORK_API DSFVoiceCallback final : public IXAudio2VoiceCallback
{
public:
	HANDLE event;
	DSFVoiceCallback();
	~DSFVoiceCallback();
	void STDMETHODCALLTYPE OnStreamEnd() override;
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override;
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samples) override;
	void STDMETHODCALLTYPE OnBufferEnd(void* context) override;
	void STDMETHODCALLTYPE OnBufferStart(void* context) override;
	void STDMETHODCALLTYPE OnLoopEnd(void* context) override;
	void STDMETHODCALLTYPE OnVoiceError(void* context, HRESULT Error) override;
};
