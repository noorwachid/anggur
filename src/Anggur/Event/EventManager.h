#pragma once

#include "Event.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace Anggur {
    using EventListener = std::function<void (const Event&)>;
    using EventListenerMap = std::unordered_map<std::string, std::vector<EventListener> >;

    class EventManager {
    public:
        static void listen(const std::string& name, const EventListener& listener) {
            map[name].push_back(listener);
        }

        static void emit(const Event& event) {
            if (map.count(event.name)) {
                std::vector<EventListener>& listeners = map[event.name];

                for (EventListener& listener: listeners)
                    listener(event);
            }
        }

    private:
        static EventListenerMap map;
    };
}
