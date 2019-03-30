#include "DSFThreadPool.h"
#include "DSFLogging.h"

DSFThreadPool::DSFThreadPool(size_t threads)
	: stop(false)
{
	for (size_t i = 0; i < threads; ++i)
		workers.emplace_back(
			[this]
			{
				LOG_TRACE << "New worker thread at 0x" << boost::this_thread::get_id() << " created.";
				for (;;)
				{
					std::function<void()> task;

					{
						boost::unique_lock<boost::mutex> lock(this->queue_mutex);
						// Wait if tasks is empty and the pool is not stopped
						this->condition.wait(lock,
							[this] { return this->stop || !this->tasks.empty(); });
						if (this->stop && this->tasks.empty())
						{
							LOG_TRACE << "Thread Pool thread at 0x" << boost::this_thread::get_id() << " stopped.";
							return;
						}
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}

					task();
				}
			}
			);
}

DSFThreadPool::~DSFThreadPool()
{
	{
		boost::unique_lock<boost::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (boost::thread& worker : workers)
		worker.join();
}

void DSFThreadPool::Enqueue(std::function<void()> f)
{
	{
		boost::unique_lock<boost::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		if (stop)
			throw std::runtime_error("Enqueue on stopped DSFThreadPool");

		tasks.emplace(f);
	}
	condition.notify_one();
}
