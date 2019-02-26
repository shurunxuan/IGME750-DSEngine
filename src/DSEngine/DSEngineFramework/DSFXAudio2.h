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

