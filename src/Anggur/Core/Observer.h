#pragma once

#include <string>

namespace Anggur
{
	struct Event
	{
		bool used = false;
	};

	class Observer
	{
	public:
		virtual void OnNotify(Event& event) {}
	};
}
