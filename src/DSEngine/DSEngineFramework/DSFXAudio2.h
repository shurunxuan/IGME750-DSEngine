/**
 * @file DSFXAudio2.h
 * @author Victor Shu
 * @brief This file declares the framework class for XAudio2.
 * @version 0.1
 * @date 2019/02/26
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <string>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @brief The XAudio2 Framework of the DS Engine
 */
class DSENGINEFRAMEWORK_API DSFXAudio2
{
public:
	/**
	 * @brief Construct a new DSFXAudio2 object
	 * 
     * Should initialize all pointers as nullptr
	 */
	DSFXAudio2();
	/**
	 * @brief Destroy the DSFXAudio2 object
	 * 
     * Should delete or release or free all pointers
	 */
	~DSFXAudio2();

	/**
     * @brief Copy constructor of DSFXAudio2 is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXAudio2(const DSFXAudio2& v) = delete;

    /**
     * @brief Move constructor of DSFXAudio2 is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXAudio2(DSFXAudio2&& v) = delete;

    /**
     * @brief Copy assignment operator of DSFXAudio2 is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXAudio2& operator=(const DSFXAudio2& v) = delete;

    /**
     * @brief Move assignment operator of DSFXAudio2 is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXAudio2& operator=(DSFXAudio2&& v) = delete;

	/**
	 * @brief Actual initialization of XAudio2 Framework
	 * 
	 */
	void Init();

	/**
	 * @brief Create the XAudio2 Engine
	 * 
	 * @return HRESULT S_OK if creation succeed, or other
	 */
	HRESULT CreateXAudio2Engine();

	/**
	 * @brief Create the mastering voice
	 * 
	 * @return HRESULT S_OK if creation succeed, or other
	 */
	HRESULT CreateMasteringVoice();

	/**
	 * @brief Get the Mastering Voice Sample Rate
	 * 
	 * @return unsigned int Mastering voice sample rate
	 */
	unsigned int GetMasteringVoiceSampleRate() const;

	/**
	 * @brief Get the Mastering Voice Channel
	 * 
	 * @return unsigned int Mastering voice channel
	 */
	unsigned int GetMasteringVoiceChannel() const;

	/**
	 * @brief Create the source voice
	 * 
	 * @param ppSourceVoice The pointer to an output source voice pointer
	 * @param channels The target channels of the source voice
	 * @param sampleRate The target sample rate of the source voice
	 * @param bytesPerSample The target bytes per sample of the source voice
	 * @param pCallback The pointer to the client defined callback class, default to nullptr
	 *
	 * @return HRESULT S_OK if creation succeed, or other
	 */
	HRESULT CreateSourceVoice(IXAudio2SourceVoice** ppSourceVoice, int channels, int sampleRate, int bytesPerSample, IXAudio2VoiceCallback* pCallback = nullptr) const;

private:
	/**
	 * @brief XAudio2 Engine
	 * 
	 */
	IXAudio2* xAudio2;
	/**
	 * @brief Mastering Voice
	 * 
	 */
	IXAudio2MasteringVoice* masterVoice;
};



class DSENGINEFRAMEWORK_API DSFVoiceCallback final : public IXAudio2VoiceCallback
{
public:
	HANDLE bufferEvent;
	HANDLE streamEvent;
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
