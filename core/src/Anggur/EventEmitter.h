#pragma once

#include "Event.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace Anggur 
{
    using EventListener = std::function<void (Event&)>;
    using EventListenerMap = std::unordered_map<std::string, std::vector<EventListener> >;

    class EventEmitter 
    {
    public:
        void Listen(const std::string& name, const EventListener& listener) 
        {
            map[name].push_back(listener);
        }

        void Dismiss(const EventListener& listener) 
        {
        }

        void Emit(Event& event) 
        {
            if (map.count(event.name)) 
            {
                std::vector<EventListener>& listeners = map[event.name];

                for (EventListener& listener: listeners)
                    listener(event);
            }
        }

    private:
        EventListenerMap map;
    };
}