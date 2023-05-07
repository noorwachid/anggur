#include "Anggur/Common/Worker.h"

namespace Anggur
{
	Worker::Worker(std::size_t numThreads) : stop(false) 
	{
        threads.reserve(numThreads);

        for (std::size_t i = 0; i < numThreads; ++i) 
		{
            threads.emplace_back([this] 
			{
                while (true) 
				{
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        if (!stop && tasks.empty()) 
						{
                            condition.wait_for(lock, std::chrono::milliseconds(10));
                        }

                        if (stop && tasks.empty()) 
						{
                            return;
                        }

                        if (!tasks.empty()) 
						{
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                    }

                    if (task) 
					{
                        task();
                    }
                }
            });
        }
    }

    Worker::~Worker() 
	{
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop = true;
        }

        condition.notify_all();

        for (std::thread& thread : threads) 
		{
            thread.join();
        }
    }

    void Worker::Add(Task task) 
	{
        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.push(std::move(task));
        }

        condition.notify_one();
    }
}
