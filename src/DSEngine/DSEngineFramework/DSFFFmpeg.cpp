#include "DSFFFmpeg.h"
#include "DSFLogging.h"

#define MAX_BUFFER_COUNT XAUDIO2_MAX_QUEUED_BUFFERS 
//#define MAX_BUFFER_COUNT 2
#define MAX_AUDIO_FRAME_SIZE 384000
DSFFFmpeg::DSFFFmpeg()
{
	formatContext = nullptr;
	audioStream = nullptr;
	codec = nullptr;
	codecContext = nullptr;
	frame = nullptr;
	lastFrame = nullptr;
	swr = nullptr;
	packet = {};
	swrBufferLength = MAX_AUDIO_FRAME_SIZE;
	swrBuffer = new BYTE[MAX_AUDIO_FRAME_SIZE];
	buffer = new BYTE * [MAX_BUFFER_COUNT];
	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
		buffer[i] = new BYTE[MAX_AUDIO_FRAME_SIZE];
	bufferCount = 0;
	eof = false;
}


DSFFFmpeg::~DSFFFmpeg()
{
	avformat_free_context(formatContext);
	avcodec_free_context(&codecContext);
	if (swr != nullptr)
		swr_free(&swr);
	av_packet_unref(&packet);
	if (lastFrame != nullptr)
		av_frame_free(&lastFrame);
	av_frame_unref(frame);
	if (!eof)
	{
		av_frame_free(&frame);
	}
	delete[] swrBuffer;

	for (int i = 0; i < MAX_BUFFER_COUNT; ++i)
		delete[] buffer[i];
	delete[] buffer;
}

void DSFFFmpeg::Init()
{
	formatContext = avformat_alloc_context();

	LOG_TRACE << "DS Engine Framework for FFmpeg Initialized!";
}

int DSFFFmpeg::OpenFile(const char* filename)
{
	int ret = avformat_open_input(&formatContext, filename, nullptr, nullptr);
	if (ret < 0)
	{
		LOG_ERROR << "FFmpeg can't open file " << filename << ", error " << AVERROR(ret);
		return ret;
	}

	ret = avformat_find_stream_info(formatContext, nullptr);
	if (ret < 0) {
		LOG_ERROR << "avformat_find_stream_info error: " << AVERROR(ret);
		return ret;
	}

	// Find the first audio stream
	for (unsigned int i = 0; i < formatContext->nb_streams; i++)
		if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = formatContext->streams[i];
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
	codecContext = avcodec_alloc_context3(codec);
	if (codecContext == nullptr) {
		LOG_ERROR << "avcodec_alloc_context3 error.";
		return -1;
	}

	avcodec_parameters_to_context(codecContext, audioStream->codecpar);
	codecContext->channel_layout = av_get_default_channel_layout(codecContext->channels);

	ret = avcodec_open2(codecContext, codec, nullptr);
	if (ret < 0) {
		LOG_ERROR << "avcodec_open2 error: " << AVERROR(ret);
		return -1;
	}

	LOG_TRACE << "File " << filename << " opened.";
	LOG_INFO << "Decoding with " << codec->long_name;
	return 0;
}

int DSFFFmpeg::ReadFrame()
{
	int ret = 0;

	do
	{
		av_packet_unref(&packet);
		if ((ret = av_read_frame(formatContext, &packet)) < 0) {
			if (AVERROR(ret) == AVERROR(AVERROR_EOF))
			{
				av_packet_unref(&packet);
				lastFrame = frame;
				swrBufferLength = lastFrame->nb_samples * lastFrame->channels * 2; /* the 2 means S16 */
				LOG_TRACE << "End of audio file";
			}
			else
			{
				LOG_ERROR << "av_read_frame error: " << AVERROR(ret);
			}
			return ret;
		}
	} while (packet.stream_index != audioStream->index);

	// decode ES
	if ((ret = avcodec_send_packet(codecContext, &packet)) < 0) {
		LOG_ERROR << "avcodec_send_packet error: " << AVERROR(ret);
	}
	av_packet_unref(&packet);
	lastFrame = frame;
	frame = av_frame_alloc();
	if ((ret = avcodec_receive_frame(codecContext, frame)) < 0) {
		if (ret != AVERROR(EAGAIN)) {
			LOG_ERROR << "avcodec_receive_frame error: " << AVERROR(ret);
			return ret;
		}
	}

	if (lastFrame == nullptr)
		swrBufferLength = 0;
	else
		swrBufferLength = lastFrame->nb_samples * lastFrame->channels * av_get_bytes_per_sample(av_get_packed_sample_fmt(AVSampleFormat(lastFrame->format))); /* the 2 means S16 */
	return ret;
}

void DSFFFmpeg::InitSoftwareResampler(int* channels, int* sampleRate, int* bytesPerSample)
{
	ReadFrame();
	int ret = 0;
	swr = swr_alloc();
	if (swr == nullptr) {
		LOG_ERROR << "swr_alloc error";
		return;
	}
	AVSampleFormat outputFormat = av_get_packed_sample_fmt(AVSampleFormat(frame->format));
	if (av_get_packed_sample_fmt(AVSampleFormat(frame->format)) == AV_SAMPLE_FMT_FLT)
		outputFormat = AV_SAMPLE_FMT_S32;
	else if (av_get_packed_sample_fmt(AVSampleFormat(frame->format)) == AV_SAMPLE_FMT_DBL)
		outputFormat = AV_SAMPLE_FMT_S64;

	av_opt_set_int(swr, "in_channel_layout", codecContext->channel_layout, 0);
	av_opt_set_int(swr, "out_channel_layout", codecContext->channel_layout, 0);
	av_opt_set_int(swr, "in_sample_rate", codecContext->sample_rate, 0);
	av_opt_set_int(swr, "out_sample_rate", codecContext->sample_rate, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", AVSampleFormat(frame->format), 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", outputFormat, 0);
	ret = swr_init(swr);
	if (ret < 0) {
		LOG_ERROR << "swr_init error: " << AVERROR(ret);
		return;
	}
	*channels = frame->channels;
	*sampleRate = frame->sample_rate;
	*bytesPerSample = av_get_bytes_per_sample(outputFormat);
}

int DSFFFmpeg::ResampleFrame()
{
	if (lastFrame == nullptr) return AVERROR_EOF;
	int ret = swr_convert(swr, &swrBuffer, lastFrame->nb_samples, const_cast<const uint8_t * *>(lastFrame->data), lastFrame->nb_samples);
	av_frame_unref(lastFrame);
	av_frame_free(&lastFrame);
	if (ret < 0) {
		LOG_ERROR << "swr_convert error: " << AVERROR(ret);
	}
	return ret;
}

int DSFFFmpeg::SendBuffer(IXAudio2SourceVoice * sourceVoice)
{

	XAUDIO2_VOICE_STATE voiceState = {};
	sourceVoice->GetState(&voiceState);
	while (voiceState.BuffersQueued < MAX_BUFFER_COUNT)
	{
		UINT flag = 0;
		int ret = ReadFrame();
		eof = AVERROR(ret) == AVERROR(AVERROR_EOF);
		if (ret < 0 && !eof)
		{
			return ret;
		}
		ret = ResampleFrame();
		if (ret < 0 && !eof) return ret;
		if (eof)
		{
			flag = XAUDIO2_END_OF_STREAM;
		}
		memcpy(buffer[bufferCount], swrBuffer, swrBufferLength);
		XAUDIO2_BUFFER xAudio2Buffer = { flag };
		xAudio2Buffer.AudioBytes = swrBufferLength;
		xAudio2Buffer.pAudioData = buffer[bufferCount];
		sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
		sourceVoice->GetState(&voiceState);
		if (MAX_BUFFER_COUNT <= ++bufferCount)
			bufferCount = 0;
		if (eof) return AVERROR_EOF;
	}
	return 0;
}
