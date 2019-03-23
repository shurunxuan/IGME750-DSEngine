#include "DSFXAudio2.h"
#include "DSFLogging.h"

DSFXAudio2* FXAudio2 = nullptr;

DSFXAudio2::DSFXAudio2()
{
	FXAudio2 = this;

	xAudio2 = nullptr;
	masterVoice = nullptr;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
}


DSFXAudio2::~DSFXAudio2()
{
	SAFE_RELEASE(xAudio2);
	// TODO: Maybe masterVoice need to be destroyed.
}

void DSFXAudio2::Init()
{
	HRESULT hr = CreateXAudio2Engine();
	if (SUCCEEDED(hr)) hr = CreateMasteringVoice();
	if (SUCCEEDED(hr))
		LOG_TRACE << "DS Engine Framework for XAudio2 Initialized!";
}

HRESULT DSFXAudio2::CreateXAudio2Engine()
{
	HRESULT hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	XAUDIO2_DEBUG_CONFIGURATION debugConfig = {};
	debugConfig.TraceMask |= XAUDIO2_LOG_DETAIL | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_TIMING | XAUDIO2_LOG_MEMORY | XAUDIO2_LOG_STREAMING;
	xAudio2->SetDebugConfiguration(&debugConfig);
	LOG_TRACE << "XAudio2 Engine Created.";
	return hr;
}

HRESULT DSFXAudio2::CreateMasteringVoice()
{
	HRESULT hr = xAudio2->CreateMasteringVoice(&masterVoice);
	XAUDIO2_VOICE_DETAILS voiceDetails = {};
	masterVoice->GetVoiceDetails(&voiceDetails);
	LOG_TRACE << "Mastering voice created with " << GetMasteringVoiceChannel() << " channel(s), sample rate " << GetMasteringVoiceSampleRate() << "Hz.";
	return hr;
}

unsigned int DSFXAudio2::GetMasteringVoiceSampleRate() const
{
	XAUDIO2_VOICE_DETAILS voiceDetails = {};
	masterVoice->GetVoiceDetails(&voiceDetails);
	return voiceDetails.InputSampleRate;
}

unsigned int DSFXAudio2::GetMasteringVoiceChannel() const
{
	XAUDIO2_VOICE_DETAILS voiceDetails = {};
	masterVoice->GetVoiceDetails(&voiceDetails);
	return voiceDetails.InputChannels;
}

HRESULT DSFXAudio2::CreateSourceVoice(IXAudio2SourceVoice** ppSourceVoice, int channels, int sampleRate, int bytesPerSample, IXAudio2VoiceCallback* pCallback) const
{
	WAVEFORMATEXTENSIBLE wfx;
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

	wfx.Format.wFormatTag = WAVE_FORMAT_PCM;
	wfx.Format.nSamplesPerSec = sampleRate;
	wfx.Format.nChannels = channels;
	wfx.Format.wBitsPerSample = bytesPerSample * 8;
	wfx.Format.nBlockAlign = wfx.Format.nChannels * bytesPerSample;
	wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
	wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	wfx.Samples.wValidBitsPerSample = wfx.Format.wBitsPerSample;
	masterVoice->GetChannelMask(&wfx.dwChannelMask);
	wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
	
	const HRESULT hr = xAudio2->CreateSourceVoice(ppSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx), 0, 2, pCallback);
	if (FAILED(hr))
		LOG_ERROR << "Failed to create source voice";
	return hr;
}

DSFVoiceCallback::DSFVoiceCallback() :
	bufferEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr)),
	streamEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
{

}

DSFVoiceCallback::~DSFVoiceCallback()
{
	CloseHandle(bufferEvent);
	CloseHandle(streamEvent);
}

void DSFVoiceCallback::OnStreamEnd()
{
	SetEvent(streamEvent);
}

void DSFVoiceCallback::OnVoiceProcessingPassEnd()
{
}

void DSFVoiceCallback::OnVoiceProcessingPassStart(UINT32 samples)
{
}

void DSFVoiceCallback::OnBufferEnd(void* context)
{
	SetEvent(bufferEvent);
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
