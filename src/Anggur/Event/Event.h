#pragma once

#include <string>

namespace Anggur {
    struct Event {
        std::string name;

        Event(const std::string& newName): name(newName) {
        }

        virtual ~Event() {
        }
    };
}