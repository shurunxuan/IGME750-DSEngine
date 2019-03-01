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
	bufferIndex = 0;
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
	// Allocate the format context
	formatContext = avformat_alloc_context();

	LOG_TRACE << "DS Engine Framework for FFmpeg Initialized!";
}

int DSFFFmpeg::OpenFile(const char* filename)
{
	// Open the file, save the format in format context
	int ret = avformat_open_input(&formatContext, filename, nullptr, nullptr);
	if (ret < 0)
	{
		LOG_ERROR << "FFmpeg can't open file " << filename << ", error " << AVERROR(ret);
		return ret;
	}

	// Get the stream info
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

	// Get the decoder
	codec = avcodec_find_decoder(audioStream->codecpar->codec_id);

	if (codec == nullptr) {
		LOG_ERROR << "avcodec_find_decoder codec not found. codec_id=" << audioStream->codecpar->codec_id;
		return -1;
	}

	// Allocate the codec context
	codecContext = avcodec_alloc_context3(codec);
	if (codecContext == nullptr) {
		LOG_ERROR << "avcodec_alloc_context3 error.";
		return -1;
	}

	// WAV file fix: manually fill the codec parameters
	avcodec_parameters_to_context(codecContext, audioStream->codecpar);
	codecContext->channel_layout = av_get_default_channel_layout(codecContext->channels);

	// Open the codec
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

	// Keep finding the packet until it's from the audio stream
	do
	{
		// Unreferencing audio packet
		av_packet_unref(&packet);
		// Read frame
		if ((ret = av_read_frame(formatContext, &packet)) < 0) {
			if (AVERROR(ret) == AVERROR(AVERROR_EOF))
			{
				// End of file
				av_packet_unref(&packet);
				lastFrame = frame;
				swrBufferLength =
					lastFrame->nb_samples * lastFrame->channels *
					av_get_bytes_per_sample(av_get_packed_sample_fmt(AVSampleFormat(lastFrame->format)));
				LOG_TRACE << "End of audio file";
			}
			else
			{
				// Other errors
				LOG_ERROR << "av_read_frame error: " << AVERROR(ret);
			}
			return ret;
		}
	} while (packet.stream_index != audioStream->index);

	// Send packet to decoder
	if ((ret = avcodec_send_packet(codecContext, &packet)) < 0) {
		LOG_ERROR << "avcodec_send_packet error: " << AVERROR(ret);
	}

	// We don't need the packet anymore, unreferencing it
	av_packet_unref(&packet);

	// Record the old frame and allocate a new one
	lastFrame = frame;
	frame = av_frame_alloc();

	// Get the decoded frame from decoder
	if ((ret = avcodec_receive_frame(codecContext, frame)) < 0) {
		if (ret != AVERROR(EAGAIN)) {
			LOG_ERROR << "avcodec_receive_frame error: " << AVERROR(ret);
			return ret;
		}
	}

	// If this is the first read for getting frame data,don't set buffer length
	if (lastFrame == nullptr)
	{
		swrBufferLength = 0;
	}
	else
	{
		// Buffer length = samples in frame * channels * bytes per sample
		swrBufferLength = lastFrame->nb_samples * lastFrame->channels *
			av_get_bytes_per_sample(av_get_packed_sample_fmt(AVSampleFormat(lastFrame->format)));
	}
	return ret;
}

void DSFFFmpeg::InitSoftwareResampler(int* channels, int* sampleRate, int* bytesPerSample)
{
	// Read a frame first to get frame format
	ReadFrame();
	int ret = 0;

	// Allocate new software resampler
	swr = swr_alloc();

	if (swr == nullptr) {
		LOG_ERROR << "swr_alloc error";
		return;
	}

	// Format fix: Audio files like MP3 use floating numbers to store data.
	// The XAudio2, however, requires signed int.
	// If the file uses floating number, resample it as signed int with the same size.
	AVSampleFormat outputFormat = av_get_packed_sample_fmt(AVSampleFormat(frame->format));
	// float -> 32 bit signed int
	if (av_get_packed_sample_fmt(AVSampleFormat(frame->format)) == AV_SAMPLE_FMT_FLT)
		outputFormat = AV_SAMPLE_FMT_S32;
	// double -> 64 bit signed int
	else if (av_get_packed_sample_fmt(AVSampleFormat(frame->format)) == AV_SAMPLE_FMT_DBL)
		outputFormat = AV_SAMPLE_FMT_S64;

	// Set the input->output parameters
	// We are not actually resampling the frame
	// Because of a potential bug in the FFmpeg, we 
	// let XAudio2 mastering voice do this for us.
	// That's also why we need to output the 
	// parameters to create the source voice.
	av_opt_set_int(swr, "in_channel_layout", codecContext->channel_layout, 0);
	av_opt_set_int(swr, "out_channel_layout", codecContext->channel_layout, 0);
	av_opt_set_int(swr, "in_sample_rate", codecContext->sample_rate, 0);
	av_opt_set_int(swr, "out_sample_rate", codecContext->sample_rate, 0);

	// However, if we are facing formats that is not
	// accepted by XAudio2, we do need to resample it.
	// XAudio2 only accepts non-planar (packed), signed int format.
	// We only need to make sure that the format size (bytes per sample) is the same.
	av_opt_set_sample_fmt(swr, "in_sample_fmt", AVSampleFormat(frame->format), 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", outputFormat, 0);
	ret = swr_init(swr);

	if (ret < 0) {
		LOG_ERROR << "swr_init error: " << AVERROR(ret);
		return;
	}

	// Output the parameters for further usage
	*channels = frame->channels;
	*sampleRate = frame->sample_rate;
	*bytesPerSample = av_get_bytes_per_sample(outputFormat);
}

int DSFFFmpeg::ResampleFrame()
{
	// First resample, lastFrame is null, do nothing
	if (lastFrame == nullptr) return -1;
	// Resample the frame
	int ret = swr_convert(swr, &swrBuffer, lastFrame->nb_samples, const_cast<const uint8_t**>(lastFrame->data), lastFrame->nb_samples);
	// We got the resampled frame buffer thus we don't need the frame anymore.
	av_frame_unref(lastFrame);
	av_frame_free(&lastFrame);

	if (ret < 0) {
		LOG_ERROR << "swr_convert error: " << AVERROR(ret);
	}
	return ret;
}

int DSFFFmpeg::SendBuffer(IXAudio2SourceVoice * sourceVoice)
{
	// Get state of the source voice
	XAUDIO2_VOICE_STATE voiceState = {};
	sourceVoice->GetState(&voiceState);
	// If the buffer queue is not filled
	while (voiceState.BuffersQueued < MAX_BUFFER_COUNT)
	{
		UINT flag = 0;
		// Read & decode frame
		int ret = ReadFrame();
		// If end of file, return
		eof = AVERROR(ret) == AVERROR(AVERROR_EOF);
		if (ret < 0 && !eof)
		{
			return ret;
		}
		// Resample the frame
		ret = ResampleFrame();
		if (ret < 0 && !eof) return ret;
		if (eof)
		{
			// Set the flag to end the source voice stream
			flag = XAUDIO2_END_OF_STREAM;
		}
		// Copy the buffer
		memcpy(buffer[bufferIndex], swrBuffer, swrBufferLength);
		// Submit the buffer
		XAUDIO2_BUFFER xAudio2Buffer = { flag };
		xAudio2Buffer.AudioBytes = swrBufferLength;
		xAudio2Buffer.pAudioData = buffer[bufferIndex];
		sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
		// Get the state for the queued buffer count
		sourceVoice->GetState(&voiceState);
		// Change the buffer in use
		if (MAX_BUFFER_COUNT <= ++bufferIndex)
			bufferIndex = 0;
		if (eof) return AVERROR_EOF;
	}
	return 0;
}
