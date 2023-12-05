#include "anggur/log.h"

namespace Anggur
{
	void Log(const std::string& message)
	{
		static std::ofstream output("log.txt");
		output << message << "\n";
	}
}
