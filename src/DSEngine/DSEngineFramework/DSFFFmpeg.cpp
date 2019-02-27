#include "DSFFFmpeg.h"
#include "DSFLogging.h"

#define MAX_BUFFER_COUNT XAUDIO2_MAX_QUEUED_BUFFERS 
#define MAX_AUDIO_FRAME_SIZE 192000
DSFFFmpeg::DSFFFmpeg()
{
	formatCtx = nullptr;
	audioStream = nullptr;
	codec = nullptr;
	codecCtx = nullptr;
	frame = nullptr;
	swr = nullptr;
	packet = {};
	swr_buf_len = MAX_AUDIO_FRAME_SIZE;
	swr_buf = new BYTE[MAX_AUDIO_FRAME_SIZE];
	sourceVoice = nullptr;
	eof = false;
	buf = new BYTE * [MAX_BUFFER_COUNT];
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
		buf[i] = new BYTE[MAX_AUDIO_FRAME_SIZE];
	buf_cnt = 0;
}


DSFFFmpeg::~DSFFFmpeg()
{
	avformat_free_context(formatCtx);
	avcodec_free_context(&codecCtx);
	swr_free(&swr);
	av_packet_unref(&packet);
	av_frame_free(&frame);
	delete[] swr_buf;

	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
		delete[] buf[i];
	delete[] buf;
}

void DSFFFmpeg::Init()
{
	/*av_register_all();*/
	formatCtx = avformat_alloc_context();

	LOG_TRACE << "DS Engine Framework for FFmpeg Initialized!";
}

int DSFFFmpeg::OpenFile(const char* filename)
{
	int ret = avformat_open_input(&formatCtx, filename, nullptr, nullptr);
	if (ret < 0)
	{
		LOG_ERROR << "FFmpeg can't open file " << filename << ", error " << AVERROR(ret);
		return ret;
	}

	ret = avformat_find_stream_info(formatCtx, nullptr);
	if (ret < 0) {
		LOG_ERROR << "avformat_find_stream_info error: " << AVERROR(ret);
		return ret;
	}

	// Find the first audio stream
	for (unsigned int i = 0; i < formatCtx->nb_streams; i++)
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = formatCtx->streams[i];
			break;
		}

	if (audioStream == nullptr) {
		LOG_ERROR << "Didn't find a audio stream.";
		return -1;
	}

	codec = avcodec_find_decoder(audioStream->codecpar->codec_id);
	if (codec == nullptr) {
		LOG_ERROR << "avcodec_find_decoder codec not found. codec_id=" << audioStream->codecpar->codec_id;
		return -1;
	}

	codecCtx = avcodec_alloc_context3(codec);
	if (codecCtx == nullptr) {
		LOG_ERROR << "avcodec_alloc_context3 error.";
		return -1;
	}

	ret = avcodec_open2(codecCtx, codec, nullptr);
	if (ret < 0) {
		LOG_ERROR << "avcodec_open2 error: " << AVERROR(ret);
		return -1;
	}

	LOG_TRACE << "File " << filename << " opened.";
	return 0;
}

int DSFFFmpeg::ReadFrame()
{
	int ret = 0;
	
	do
	{
		av_packet_unref(&packet);
		if ((ret = av_read_frame(formatCtx, &packet)) < 0) {
			LOG_ERROR << "av_read_frame eof or error: " << AVERROR(ret);
			LOG_INFO << "AVERROR_EOF = " << AVERROR(AVERROR_EOF);
			return ret;
		}
	} while (packet.stream_index != audioStream->index);

	// decode ES
	if ((ret = avcodec_send_packet(codecCtx, &packet)) < 0) {
		LOG_ERROR << "avcodec_send_packet error: " << AVERROR(ret);
	}
	av_packet_unref(&packet);
	frame = av_frame_alloc();
	if ((ret = avcodec_receive_frame(codecCtx, frame)) < 0) {
		if (ret != AVERROR(EAGAIN)) {
			LOG_ERROR << "avcodec_receive_frame error: " << AVERROR(ret);
			return ret;
		}
	}
	
	//ret = swr_init(swr);
	swr_buf_len = frame->nb_samples * frame->channels * 2; /* the 2 means S16 */

	return ret;
}

void DSFFFmpeg::InitSoftwareResampler(int channel, int sampleRate)
{
	ReadFrame();
	int ret = 0;
	swr = swr_alloc();
	if (swr == nullptr) {
		LOG_ERROR << "swr_alloc error";
		return;
	}
	av_opt_set_int(swr, "in_channel_layout", frame->channel_layout, 0);
	av_opt_set_int(swr, "out_channel_layout", frame->channel_layout, 0);
	av_opt_set_int(swr, "in_sample_rate", frame->sample_rate, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", AVSampleFormat(frame->format), 0);
	av_opt_set_int(swr, "out_sample_rate", sampleRate, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	ret = swr_init(swr);

	if (ret < 0) {
		LOG_ERROR << "swr_init error: " << AVERROR(ret);
		return;
	}
}

int DSFFFmpeg::ResampleFrame()
{
	int ret = swr_convert(swr, &swr_buf, frame->nb_samples, const_cast<const uint8_t * *>(frame->data), frame->nb_samples);
	av_frame_unref(frame);
	av_frame_free(&frame);
	if (ret < 0) {
		LOG_ERROR << "swr_convert error: " << AVERROR(ret);
	}
	return ret;
}

void DSFFFmpeg::SetXAudio2SourceVoice(IXAudio2SourceVoice * sourceVoice)
{
	this->sourceVoice = sourceVoice;
}

int DSFFFmpeg::BufferEnd()
{
	XAUDIO2_VOICE_STATE voiceState = {};
	sourceVoice->GetState(&voiceState);
	while (voiceState.BuffersQueued < MAX_BUFFER_COUNT && !eof)
	{
		int ret = ReadFrame();
		if (ret < 0 && AVERROR(ret) != AVERROR(AVERROR_EOF))
		{
			return ret;
		}
		eof = AVERROR(ret) == AVERROR(AVERROR_EOF);

		int flag;
		if (!eof)
		{
			ret = ResampleFrame();
			if (ret < 0) return ret;
			memcpy(buf[buf_cnt], swr_buf, swr_buf_len);
			flag = 0;
		}		
		else
		{
			memset(buf[buf_cnt], 0, MAX_AUDIO_FRAME_SIZE);
			flag = XAUDIO2_END_OF_STREAM;
		}
		XAUDIO2_BUFFER buffer = { flag };
		buffer.AudioBytes = swr_buf_len;
		buffer.pAudioData = buf[buf_cnt];

		sourceVoice->SubmitSourceBuffer(&buffer);
		sourceVoice->GetState(&voiceState);
		//LOG_INFO << "OnBufferEnd";
		if (MAX_BUFFER_COUNT <= ++buf_cnt)
			buf_cnt = 0;
	}
	return 0;
}

DSFVoiceCallback::DSFVoiceCallback() : event(CreateEvent(nullptr, FALSE, FALSE, nullptr))
{

}

DSFVoiceCallback::~DSFVoiceCallback()
{
	CloseHandle(event);
}

void DSFVoiceCallback::OnStreamEnd()
{
}

void DSFVoiceCallback::OnVoiceProcessingPassEnd()
{
}

void DSFVoiceCallback::OnVoiceProcessingPassStart(UINT32 samples)
{
}

void DSFVoiceCallback::OnBufferEnd(void* context)
{
	SetEvent(event);
}

void DSFVoiceCallback::OnBufferStart(void* context)
{
}

void DSFVoiceCallback::OnLoopEnd(void* context)
{
}

void DSFVoiceCallback::OnVoiceError(void* context, HRESULT Error)
{
}
