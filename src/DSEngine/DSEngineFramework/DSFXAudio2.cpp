#include "DSFXAudio2.h"
#include "DSFLogging.h"
#include "AudioListener.hpp"
#include "AudioSource.hpp"

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
	if (!(SUCCEEDED(hr))) return;
	hr = CreateMasteringVoice();
	if (!(SUCCEEDED(hr))) return;
	hr = CreateX3DAudioInstance();
	if (!(SUCCEEDED(hr))) return;
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
	voiceDetails = {};
	masterVoice->GetVoiceDetails(&voiceDetails);
	LOG_TRACE << "Mastering voice created with " << GetMasteringVoiceChannel() << " channel(s), sample rate " << GetMasteringVoiceSampleRate() << "Hz.";
	return hr;
}

HRESULT DSFXAudio2::CreateX3DAudioInstance()
{
	DWORD dwChannelMask;
	masterVoice->GetChannelMask(&dwChannelMask);

	return X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
}

unsigned int DSFXAudio2::GetMasteringVoiceSampleRate() const
{
	return voiceDetails.InputSampleRate;
}

unsigned int DSFXAudio2::GetMasteringVoiceChannel() const
{
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

void DSFXAudio2::AudioCalculate3D(AudioListener* listener, AudioSource* emitter)
{
	X3DAudioCalculate(X3DInstance, &(listener->x3dListener), &(emitter->x3dEmitter), 
		X3DAUDIO_CALCULATE_MATRIX | 
		X3DAUDIO_CALCULATE_DOPPLER | 
		X3DAUDIO_CALCULATE_LPF_DIRECT |
		X3DAUDIO_CALCULATE_REVERB,
		&(emitter->dspSettings));

	// Transpose the matrix coefficients. 
	// TODO: NEED TO FIND WHY
	std::vector<FLOAT32> coefficientsTransposed;
	coefficientsTransposed.resize(emitter->dspSettings.SrcChannelCount * emitter->dspSettings.DstChannelCount);
	for (size_t d = 0; d < emitter->dspSettings.DstChannelCount; ++d)
		for (size_t s = 0; s < emitter->dspSettings.SrcChannelCount; ++s)
		{
			coefficientsTransposed[d * emitter->dspSettings.SrcChannelCount + s] =
				emitter->dspSettings.pMatrixCoefficients[s * emitter->dspSettings.DstChannelCount + d];
		}

	// Apply the result
	emitter->sourceVoice->SetOutputMatrix(masterVoice, emitter->dspSettings.SrcChannelCount, emitter->dspSettings.DstChannelCount, &*coefficientsTransposed.begin());
	emitter->sourceVoice->SetFrequencyRatio(emitter->dspSettings.DopplerFactor * emitter->frequencyRatio);

	XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * emitter->dspSettings.LPFDirectCoefficient), 1.0f };
	emitter->sourceVoice->SetFilterParameters(&FilterParameters);
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
