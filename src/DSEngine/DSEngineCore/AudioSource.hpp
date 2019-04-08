#pragma once
/**
 * @file AudioSource.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the audio source
 *
 * @version 0.1
 * @date 2019/04/06
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>
#include <boost/thread.hpp>
#include "DSFFFmpeg.h"
#include "DSFXAudio2.h"
#include "DSFLogging.h"
#include "Component.hpp"
#include "Object.hpp"

class AudioSource 
	: public Component
{
private:
	friend class DSFXAudio2;
	std::string filename;
	/**
	 * @brief The FFmpeg Framework reference
	 *
	 * Can only play/decode one file at a time
	 *
	 */
	DSFFFmpeg ffmpeg;
	IXAudio2SourceVoice* sourceVoice;
	// The callback will be used by the source voice
	DSFVoiceCallback callback;
	boost::thread playbackThread;

	X3DAUDIO_EMITTER x3dEmitter;
	X3DAUDIO_DSP_SETTINGS dspSettings;

	DirectX::XMVECTOR audioVelocity;
	DirectX::XMVECTOR lastPosition;

	int channels;
	int sampleRate;
	int bytesPerSample;

	float frequencyRatio;

	std::vector<FLOAT32> matrixCoefficients;
	std::vector<FLOAT32> channelAzimuths;

	void PlaySync();

	void UpdateX3DAudioSettings();
public:
	AudioSource(Object* owner);
	~AudioSource();

	void LoadAudioFile(const std::string& filename);

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	void Play();
	void Stop();
	void Pause();

	void SetFrequencyRatio(float frequencyRatio);
	void SetVolume(float volume);

	bool Loop;
	bool isPlaying;
	bool stopped;
	bool fileOpened;
	bool Is3D;
};

inline void AudioSource::PlaySync()
{
	if (!fileOpened) return;

	// Start the source voice
	sourceVoice->Start();

	LOG_TRACE << "Starting audio file \"" << filename << "\" playback at 0x" << boost::this_thread::get_id();

	bool stoppedByStreamEnd = true;

	try
	{
		while (true)
		{
			// If the source voice is going to be starved, decode & send buffer with FFmpeg
			int i = ffmpeg.SendBuffer(sourceVoice);

			boost::this_thread::interruption_point();

			// Waiting for the source voice buffer to end
			WaitForSingleObject(callback.bufferEvent, INFINITE);

			// If FFmpeg hit the end of file
			if (i != 0)
			{
				if (Loop)
				{
					// Seek to the start of the file
					ffmpeg.Seek(0, false);
				}
				else
				{
					// Break the loop
					break;
				}
			}
		}

		LOG_TRACE << "Waiting for the stream to end";

		// Waiting the source voice stream to end
		if (WaitForSingleObject(callback.streamEvent, 10000) == WAIT_TIMEOUT)
		{
			// If timeout, force the stream to end
			LOG_WARNING << "Timeout. Forcing the stream to end. Please check if the XAUDIO2_END_OF_STREAM flag is properly set.";
		}

	}
	catch (boost::thread_interrupted const& tie)
	{
		UNREFERENCED_PARAMETER(tie);

		LOG_WARNING << "Audio playback thread interrupted!";

		stoppedByStreamEnd = false;
	}

	// Stop the source voice
	sourceVoice->Stop();

	LOG_TRACE << "Stopping audio playback";

	// Clear all queued buffers
	sourceVoice->FlushSourceBuffers();

	ffmpeg.Seek(0, !stoppedByStreamEnd);

	isPlaying = false;
	stopped = true;
}

inline void AudioSource::UpdateX3DAudioSettings()
{
	x3dEmitter.ChannelCount = channels;
	channelAzimuths.resize(x3dEmitter.ChannelCount);
	for (float& channelAzimuth : channelAzimuths)
		channelAzimuth = 0.0f;
	x3dEmitter.pChannelAzimuths = &*channelAzimuths.begin();
	x3dEmitter.CurveDistanceScaler = 10.0f;
	x3dEmitter.DopplerScaler = 1.0f;
	dspSettings.SrcChannelCount = channels;
	dspSettings.DstChannelCount = FXAudio2->GetMasteringVoiceChannel();
	matrixCoefficients.resize(dspSettings.DstChannelCount * dspSettings.SrcChannelCount);
	dspSettings.pMatrixCoefficients = &*matrixCoefficients.begin();
}

inline AudioSource::AudioSource(Object* owner)
	: Component(owner)
{
	isPlaying = false;
	stopped = true;
	fileOpened = false;
	Loop = false;
	Is3D = false;
	sourceVoice = nullptr;

	frequencyRatio = 1.0f;

	ZeroMemory(&x3dEmitter, sizeof(X3DAUDIO_EMITTER));
	ZeroMemory(&dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
	UpdateX3DAudioSettings();
}

inline AudioSource::~AudioSource()
{
	if (sourceVoice != nullptr)
	{
		// Stop the source voice
		sourceVoice->Stop();

		// Clear all queued buffers
		sourceVoice->FlushSourceBuffers();

		// Destroy source voice
		sourceVoice->DestroyVoice();
	}
}

inline void AudioSource::LoadAudioFile(const std::string& filename)
{
	if (fileOpened) return;
	this->filename = filename;
	// Open file with FFmpeg, and initialize codecs
	int res = ffmpeg.OpenFile(this->filename.c_str());
	fileOpened = res >= 0;

	if (!fileOpened) return;

	// Parameters to be used when creating source voice
	// Initialize the resampler, and get the parameters required
	ffmpeg.InitSoftwareResampler(&channels, &sampleRate, &bytesPerSample);

	// Create the source voice with the parameters
	FXAudio2->CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);
	UpdateX3DAudioSettings();
}

inline void AudioSource::Start()
{
	ffmpeg.Init();

	audioVelocity = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	lastPosition = object->transform->GetGlobalTranslation();
}

inline void AudioSource::Update(float deltaTime, float totalTime)
{
	if (!Is3D) return;
	const DirectX::XMVECTOR currentPosition = object->transform->GetGlobalTranslation();
	audioVelocity = DirectX::XMVectorSubtract(currentPosition, lastPosition);
	audioVelocity = DirectX::XMVectorScale(audioVelocity, 1.0f / deltaTime);
	lastPosition = currentPosition;
	DirectX::XMStoreFloat3(&x3dEmitter.OrientFront, object->transform->Forward());
	DirectX::XMStoreFloat3(&x3dEmitter.OrientTop, object->transform->Up());
	DirectX::XMStoreFloat3(&x3dEmitter.Position, currentPosition);
	DirectX::XMStoreFloat3(&x3dEmitter.Velocity, audioVelocity);

	// TODO: Calculate
}

inline void AudioSource::Play()
{
	if (isPlaying)
	{
		return;
	}
	else 
	{
		if (stopped)
		{
			// stopped
			playbackThread = boost::thread(&AudioSource::PlaySync, this);
		}
		else 
		{
			// paused
			// resume
			sourceVoice->Start();

		}
	}
	isPlaying = true;
	stopped = false;
}

inline void AudioSource::Stop()
{
	if (isPlaying || (!isPlaying && !stopped))
	{
		SetEvent(callback.bufferEvent);
		playbackThread.interrupt();
		playbackThread.join();
	}
}

inline void AudioSource::Pause()
{
	if (!isPlaying) return;
	if (stopped) return;

	sourceVoice->Stop();

	isPlaying = false;
	stopped = false;
}

inline void AudioSource::SetFrequencyRatio(float frequencyRatio)
{
	this->frequencyRatio = frequencyRatio;
}

inline void AudioSource::SetVolume(float volume)
{
	sourceVoice->SetVolume(volume);
}
