#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace Anggur {
	class Worker {
	public:
		using Task = std::function<void()>;

		Worker(std::size_t numThreads);

		~Worker();

		void add(Task task);

	private:
		std::vector<std::thread> threads;
		std::queue<Task> tasks;
		std::mutex mutex;
		std::condition_variable condition;
		bool stop;
	};
}
