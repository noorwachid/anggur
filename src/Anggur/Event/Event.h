#pragma once

#include <cstddef>

namespace Anggur 
{
	using EventType = size_t;

	static EventType CreateEventType()
	{
		static EventType typeIncrement = 0;

		return ++typeIncrement;
	}

	template <typename T>
	EventType GetEventType()
	{
		static EventType type = CreateEventType();

		return type;
	}

	#define ANGGUR_EVENT(T)                     \
		::Anggur::EventType GetType() override  \
		{                                       \
			return ::Anggur::GetEventType<T>(); \
		}

	struct Event 
	{
		virtual ~Event() {}

		virtual EventType GetType() = 0;	
	};
}
