#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace Anggur
{
	class Worker 
	{
	public:
		using Task = std::function<void()>;

		Worker(std::size_t numThreads);

		~Worker();

		void Add(Task task);

	private:
		std::vector<std::thread> threads;
		std::queue<Task> tasks;
		std::mutex mutex;
		std::condition_variable condition;
		bool stop;
	};
}
