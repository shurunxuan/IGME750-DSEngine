/**
 * @file DSFXInput.h
 * @author Victor Shu
 * @brief This file declares the framework class for Thread Pool.
 * @version 0.1
 * @date 2019/03/28
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <boost/thread.hpp>

#pragma warning(disable:4251)

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @brief The thread pool class
 */
class DSENGINEFRAMEWORK_API DSFThreadPool {
public:
	/**
	 * @brief The constructor launches some amount of workers
	 * 
	 * @param threads The number of worker threads
	 */
	DSFThreadPool(size_t threads);
	/**
	 * @brief The destructor joins all threads
	 */
	~DSFThreadPool();
	/**
	 * @brief Add new work item to the pool
	 * 
	 * @param f The worker function
	 */
	void Enqueue(std::function<void()> f);

	friend class DSEngineApp;

private:
	/** 
	 * @brief Keep track of threads so we can join them
	 */
	std::vector< boost::thread > workers;
	/**
	 * @brief The task queue
	 */
	std::queue< std::function<void()> > tasks;

	/**
	 * @brief Synchronization mutex
	 */
	boost::mutex queue_mutex;
	/**
	 * @brief Condition variable
	 */
	boost::condition_variable condition;
	/**
	 * @brief Keep track if the pool is stopped
	 */
	bool stop;
};

