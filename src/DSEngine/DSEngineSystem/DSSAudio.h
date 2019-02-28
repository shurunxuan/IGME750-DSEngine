/**
 * @file DSSAudio.h
 * @author Victor Shu
 * @brief This file declares the class for DSEngine Audio System.
 * @version 0.1
 * @date 2019/02/26
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#include <boost/thread.hpp>
#include "DSFXAudio2.h"
#include "DSFFFmpeg.h"

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

/**
 * @brief The Audio System of the DS Engine
 * 
 */
class DSENGINESYSTEM_API DSSAudio
{
public:
	/**
	 * @brief Construct a new DSSAudio object
	 * 
	 * Should set all pointers to nullptr
	 */
	DSSAudio();
	/**
	 * @brief Destroy the DSSAudio object
	 * 
	 * Should delete or release or free all pointers
	 */
	~DSSAudio();

    /**
     * @brief Copy constructor of DSSAudio is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSSAudio(const DSSAudio& v) = delete;

    /**
     * @brief Move constructor of DSSAudio is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSSAudio(DSSAudio&& v) = delete;

    /**
     * @brief Copy assignment operator of DSSAudio is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSSAudio& operator=(const DSSAudio& v) = delete;

    /**
     * @brief Move assignment operator of DSSAudio is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSSAudio& operator=(DSSAudio&& v) = delete;

	/**
	 * @brief Actual initialization of the audio system
	 * 
	 */
	void Init();

	void OpenAudioFile(const char* filename);

	// TODO: Test function, please delete!
	void PlayAudioFile(const char* filename);
	void PlayAudioFileThread(const char* filename);

private:
	/**
	 * @brief The XAudio2 Framework reference
	 * 
	 */
	DSFXAudio2 xAudio2;

	DSFFFmpeg ffmpeg;

	boost::thread playThread;
};

