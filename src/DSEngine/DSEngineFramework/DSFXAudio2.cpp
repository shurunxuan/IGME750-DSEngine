#include "DSFXAudio2.h"
#include "DSFLogging.h"


DSFXAudio2::DSFXAudio2()
{
	xAudio2 = nullptr;
	masterVoice = nullptr;
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
	LOG_TRACE << "XAudio2 Engine Created.";
	return hr;
}

HRESULT DSFXAudio2::CreateMasteringVoice()
{
	HRESULT hr = xAudio2->CreateMasteringVoice(&masterVoice);
	XAUDIO2_VOICE_DETAILS voiceDetails = {};
	masterVoice->GetVoiceDetails(&voiceDetails);
	LOG_TRACE << "Mastering voice created with " << voiceDetails.InputChannels << " channel(s), sample rate " << voiceDetails.InputSampleRate << "Hz.";
	return hr;
}
