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

	/**
	 * @brief Non-block version of playing an audio file
     * 
     * Creates a thread of PlayAudioFile function
	 * 
	 * @param filename Audio file name
	 */
	void PlayAudioFileNonBlock(const char* filename);

    /**
     * @brief Play an audio file
     * 
     * This function will block the thread. 
     * If you want a non-block version,
     * consider calling PlayAudioFileNonBlock.
     * 
     * @param filename Audio file name
     */
	void PlayAudioFile(const char* filename);

private:
	/**
	 * @brief The XAudio2 Framework reference
	 * 
	 */
	DSFXAudio2 xAudio2;
    /**
     * @brief The FFmpeg Framework reference
     * 
     * Can only play/decode one file at a time
     * 
     */
	DSFFFmpeg ffmpeg;

    /**
     * @brief Thread for playing audio file.
     * 
     */
	boost::thread playThread;
};

