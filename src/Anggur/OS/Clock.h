#pragma once

#include <chrono>

namespace Anggur {
	using ClockContext = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<ClockContext>;

	class Clock {
	public:
		Clock() {
			started = ClockContext::now();
		}

		/**
		 * Get elapsed time on call
		 */
		float getElapsedTime() const {
			TimePoint current = ClockContext::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(current - started).count() * 0.001;
		}

		/**
		 * Get distance between previous tick
		 */
		float getDeltaTime() {
			float currentTime = getElapsedTime();
			float deltaTime = currentTime - previousTime;

			previousTime = currentTime;

			return deltaTime;
		}

	private:
		ClockContext context;
		TimePoint started;

		float previousTime = 0;
	};
}
