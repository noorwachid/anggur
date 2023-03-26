#pragma once

#include <string>

namespace Anggur
{
	struct Event
	{
		bool used = false;
	};

	struct NotificationEvent : Event
	{
		std::string name;
	};

	class NotificationEventListener
	{
	public:
		virtual void OnNotify(NotificationEvent& event) {}
	};
}
