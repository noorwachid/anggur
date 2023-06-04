#pragma once

#include "Anggur/Graphics/Animation/Function.h"
#include "Anggur/Math/Math.h"

namespace Anggur
{
	template <typename T>
	class Tween
	{
	public:
		void SetFunction(AnimationFunction function)
		{
			this->function = function;
		}

		void SetDuration(float time)
		{
			durationTime = time;
		}

		void SetFrom(const T& from)
		{
			this->from = from;
		}

		void SetTo(const T& to)
		{
			this->to = to;
		}

		const T& GetInBetween() const
		{
			return this->inBetween;
		}

		void Reset()
		{
			elapsedTime = 0.0f;
		}

		void Interupt(const T& to)
		{
			Reset();

			this->from = inBetween;
			this->to = to;
		}

		void Update(float deltaTime)
		{
			if (elapsedTime > durationTime || function == nullptr)
				return;

			elapsedTime += deltaTime;

			inBetween = Math::Lerp(from, to, function(elapsedTime / durationTime));
		}
	
	private:
		T from;
		T to;
		T inBetween;

		float durationTime = 1.0f;
		float elapsedTime = 0.0f;

		AnimationFunction function = Linear;

	};
}
