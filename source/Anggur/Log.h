#pragma once

#include <string>
#include <fstream>

namespace Anggur
{
	void Log(const std::string& message);
}

#define ANGGUR_LOG(message) Anggur::Log(message);

