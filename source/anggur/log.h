#pragma once

#include <fstream>
#include <string>

namespace Anggur
{
	void Log(const std::string& message);
}

#define ANGGUR_LOG(message) Anggur::Log(message);
