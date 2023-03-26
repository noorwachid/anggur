#pragma once

#include "Anggur/Integral.h"
#include <string>
#include <vector>

namespace Anggur::File
{
	/**
	 * Load path as array of buffer
	 */
	std::vector<uchar> Read(const std::string& path);

	/**
	 * Load path of to std::string
	 */
	std::string ReadText(const std::string& path);
}
