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

/**
 * @brief The Audio Source
 * 
 */
class AudioSource final
	: public Component
{
private:
	friend class DSFXAudio2;

	/**
	 * @brief The audio file name
	 * 
	 */
	std::string filename;
	/**
	 * @brief The FFmpeg Framework reference
	 *
	 * Can only play/decode one file at a time
	 *
	 */
	DSFFFmpeg ffmpeg;

	/**
	 * @brief The XAudio2 Source Voice
	 * 
	 */
	IXAudio2SourceVoice* sourceVoice;
	
	/**
	 * @brief The Callback that will be used by the source voice
	 * 
	 */
	DSFVoiceCallback callback;

	/**
	 * @brief The thread that is responsible for audio playback
	 * 
	 */
	boost::thread playbackThread;

	/**
	 * @brief The X3DAudio Emitter struct
	 * 
	 */
	X3DAUDIO_EMITTER x3dEmitter;

	/**
	 * @brief The X3DAudio DSP Settings
	 * 
	 */
	X3DAUDIO_DSP_SETTINGS dspSettings;

	/**
	 * @brief The global position of last frame
	 * 
	 */
	DirectX::XMFLOAT3 lastPosition;

	/**
	 * @brief The channels of the audio
	 * 
	 * Will be set properly after calling LoadAudioFile
	 * 
	 */
	int channels;
	/**
	 * @brief The sample rate of the audio
	 * 
	 * Will be set properly after calling LoadAudioFile
	 * 
	 */
	int sampleRate;
	/**
	 * @brief The bytes per sample of the audio
	 * 
	 * Will be set properly after calling LoadAudioFile
	 * 
	 */
	int bytesPerSample;

	/**
	 * @brief The frequency ratio of the audio playback
	 * 
	 */
	float frequencyRatio;
	/**
	 * @brief The volume of the audio playback
	 * 
	 */
	float volume;

	/**
	 * @brief Indicates if the audio is playing
	 * 
	 */
	bool isPlaying;
	/**
	 * @brief Indicates if the audio is completely stopped
	 * 
	 */
	bool stopped;
	/**
	 * @brief Indicates if the audio file is opened
	 * 
	 */
	bool fileOpened;

	/**
	 * @brief The 3D audio output matrix coefficients
	 * 
	 */
	std::vector<FLOAT32> matrixCoefficients;
	/**
	 * @brief The 3D audio channel azimuths
	 * 
	 */
	std::vector<FLOAT32> channelAzimuths;

	/**
	 * @brief The synchronized audio playback function
	 * 
	 */
	void PlaySync();

	/**
	 * @brief Update the X3DAudio settings with current set parameters
	 * 
	 */
	void UpdateX3DAudioSettings();
public:
	/**
	 * @brief Construct a new Audio Source object
	 * 
	 * @param owner The object that the component is attached to
	 */
	AudioSource(Object* owner);
	/**
	 * @brief Destroy the Audio Source object
	 * 
	 */
	~AudioSource();

	/**
	 * @brief Load an audio file
	 * 
	 * @param filename The audio file name
	 */
	void LoadAudioFile(const std::string& filename);

	/**
	 * @brief Initialize the FFMpeg framework
	 * 
	 */
	void Start() override;
	/**
	 * @brief Update the current position, orientation and velocity of the X3DAudio Emitter
	 * 
	 * @param deltaTime The time that a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	void Update(float deltaTime, float totalTime) override;

	/**
	 * @brief Start/resume the audio playback
	 * 
	 */
	void Play();
	/**
	 * @brief Stop the audio playback completely
	 * 
	 */
	void Stop();
	/**
	 * @brief Pause the audio playback
	 * 
	 */
	void Pause();

	/**
	 * @brief Indicates if the audio is playing
	 * 
	 * @return true if the audio is playing
	 */
	bool Playing() const;
	/**
	 * @brief Indicates if the audio is completely stopped
	 * 
	 * @return true if the audio is completely stopped
	 */
	bool Stopped() const;
	/**
	 * @brief Indicates if the audio is paused
	 * 
	 * @return true if the audio is paused
	 */
	bool Paused() const;

	/**
	 * @brief Set the Frequency Ratio of audio playback
	 * 
	 * The value should be between FLT_MIN to FLT_MAX.
	 * This will affect both the sound pitch and playback speed.
	 * A value of 1.0f means no change.
	 * For example, if a 2 times faster speed and an octave higher pitch is desired, 
	 * set the value to 2.0f.
	 * 
	 * @param frequencyRatio The target frequency ratio. Equals to sourceFrequency / targetFrequency
	 */
	void SetFrequencyRatio(float frequencyRatio);
	/**
	 * @brief Set the Volume of audio playback
	 * 
	 * The value can be any float number. A negative number means to negate the phase of the audio.
	 * A value of 1.0f means no gain. A value of 0.0f means mute.
	 * 
	 * @param volume The target volume
	 */
	void SetVolume(float volume);

	/**
	 * @brief Set the Doppler Scaler
	 * 
	 * The Doppler Effect Scaler. The bigger the value is, the more obvious the effect is.
	 * The value should be between 0 to FLT_MAX. Values out of this range will be clamped.
	 * A value of 0 means no Doppler Effect at all.
	 * The default value is 1.0f.
	 * Only used in 3D audio.
	 * 
	 * @param dopplerScaler The desired Doppler Effect scaler
	 */
	void SetDopplerScaler(float dopplerScaler);
	/**
	 * @brief Set the Curve Distance Scaler 
	 * 
	 * The Curve Distance Scaler. Distance will have more impact on volume if the value is smaller.
	 * The value should be between FLT_MIN to FLT_MAX. Values out of this range will be clamped.
	 * A value of FLT_MIN means almost mute at any position.
	 * A value of FLT_MAX means the distance has almost no effect on volume.
	 * The default value is 10.0f.
	 * Only used in 3D audio.
	 * 
	 * @param curveDistanceScaler The desired curve distance scaler
	 */
	void SetCurveDistanceScaler(float curveDistanceScaler);

	/**
	 * @brief Get the Frequency Ratio 
	 * 
	 * @return float The frequency ratio
	 */
	float GetFrequencyRatio() const;
	/**
	 * @brief Get the Volume 
	 * 
	 * @return float The volume
	 */
	float GetVolume() const;

	/**
	 * @brief Get the Doppler Scaler
	 * 
	 * @return float The doppler scaler
	 */
	float GetDopplerScaler() const;
	/**
	 * @brief Get the Curve Distance Scaler
	 * 
	 * @return float The curve distance scaler
	 */
	float GetCurveDistanceScaler() const;

	/**
	 * @brief Indicates if the audio will loop after finished
	 * 
	 * The default value is false.
	 * 
	 */
	bool Loop;
	/**
	 * @brief Indicates if the audio source is a 3D audio source
	 * 
	 * If the value is true, one and only one AudioListener component must be presented in the scene.
	 * If the value is false, no AudioListener is required and no 3D audio calculation will be applied.
	 * 
	 */
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
	if (channels <= 0) return;
	x3dEmitter.ChannelCount = channels;
	channelAzimuths.resize(x3dEmitter.ChannelCount);
	for (float& channelAzimuth : channelAzimuths)
		channelAzimuth = 0.0f;
	x3dEmitter.pChannelAzimuths = &*channelAzimuths.begin();
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
	x3dEmitter.CurveDistanceScaler = 10.0f;
	x3dEmitter.DopplerScaler = 1.0f;
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

	DirectX::XMStoreFloat3(&lastPosition, object->transform->GetGlobalTranslation());
}

inline void AudioSource::Update(float deltaTime, float totalTime)
{
	if (!Is3D) return;
	const DirectX::XMVECTOR currentPosition = object->transform->GetGlobalTranslation();
	DirectX::XMVECTOR audioVelocityVec = DirectX::XMVectorSubtract(currentPosition, DirectX::XMLoadFloat3(&lastPosition));
	audioVelocityVec = DirectX::XMVectorScale(audioVelocityVec, 1.0f / deltaTime);
	DirectX::XMStoreFloat3(&lastPosition, currentPosition);
	DirectX::XMStoreFloat3(&x3dEmitter.OrientFront, object->transform->Forward());
	DirectX::XMStoreFloat3(&x3dEmitter.OrientTop, object->transform->Up());
	DirectX::XMStoreFloat3(&x3dEmitter.Position, currentPosition);
	DirectX::XMStoreFloat3(&x3dEmitter.Velocity, audioVelocityVec);
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

inline bool AudioSource::Playing() const
{
	return isPlaying && !stopped;
}

inline bool AudioSource::Stopped() const
{
	return !isPlaying && stopped;
}

inline bool AudioSource::Paused() const
{
	return !isPlaying && !stopped;
}

inline void AudioSource::SetFrequencyRatio(float frequencyRatio)
{
	this->frequencyRatio = frequencyRatio;
	if (!Is3D)
		sourceVoice->SetFrequencyRatio(this->frequencyRatio);
}

inline void AudioSource::SetVolume(float volume)
{
	sourceVoice->SetVolume(volume);
	this->volume = volume;
}

inline void AudioSource::SetDopplerScaler(float dopplerScaler)
{
	x3dEmitter.DopplerScaler = dopplerScaler;
	if (x3dEmitter.DopplerScaler < 0)
		x3dEmitter.DopplerScaler = 0;
}

inline void AudioSource::SetCurveDistanceScaler(float curveDistanceScaler)
{
	x3dEmitter.CurveDistanceScaler = curveDistanceScaler;
	if (x3dEmitter.CurveDistanceScaler < FLT_MIN)
		x3dEmitter.CurveDistanceScaler = FLT_MIN;
}

inline float AudioSource::GetFrequencyRatio() const
{
	float result;
	sourceVoice->GetFrequencyRatio(&result);
	return result;
}

inline float AudioSource::GetVolume() const
{
	float result;
	sourceVoice->GetVolume(&result);
	return result;
}

inline float AudioSource::GetDopplerScaler() const
{
	return x3dEmitter.DopplerScaler;
}

inline float AudioSource::GetCurveDistanceScaler() const
{
	return x3dEmitter.CurveDistanceScaler;
}
