#pragma once

#include "Event.h"

namespace Anggur {
    struct LogEvent: public Event {
        std::string value;

        LogEvent(const std::string& value): Event("Log"), value(value) {
        }
    };
}