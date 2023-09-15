#pragma once

#include <fstream>
#include <string>

namespace Anggur {
	void log(const std::string& message);
}

#define ANGGUR_LOG(message) Anggur::log(message);
