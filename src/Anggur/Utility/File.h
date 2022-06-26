#pragma once

#include <vector>
#include <string>

namespace Anggur::File 
{
	/**
	 *	Load file as byte array
	 */
	std::vector<uint8_t> Load(const std::string& path);

	/**
	 * Load file as string
	 */
	std::string LoadText(const std::string& path);
}
