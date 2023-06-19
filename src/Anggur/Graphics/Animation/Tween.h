#pragma once

#include "Anggur/Graphics/Animation/Function.h"
#include "Anggur/Math/Math.h"
#include <functional>

namespace Anggur {
	using TweenCallback = std::function<void()>;

	template <typename T>
	class Tween {
	public:
		void setFunction(AnimationFunction function) {
			this->function = function;
		}

		void setCallback(const TweenCallback& callback) {
			this->callback = callback;
		}

		void setDuration(float time) {
			durationTime = time;
		}

		void setFrom(const T& from) {
			this->from = from;
		}

		void setTo(const T& to) {
			this->inBetween = to;
			this->to = to;

			if (callback)
				callback();
		}

		const T& get() const {
			return this->inBetween;
		}

		void reset() {
			elapsedTime = 0.0f;
		}

		void interupt(const T& to) {
			reset();

			this->from = inBetween;
			this->to = to;
		}

		void update(float deltaTime) {
			if (elapsedTime > durationTime || function == nullptr) {
				inBetween = to;

				if (callback)
					callback();

				return;
			}

			elapsedTime += deltaTime;

			inBetween = Math::lerp(from, to, function(elapsedTime / durationTime));
		}

	private:
		T from;
		T to;
		T inBetween;

		float durationTime = 1.0f;
		float elapsedTime = 0.0f;

		AnimationFunction function = linear;
		TweenCallback callback = nullptr;
	};
}
