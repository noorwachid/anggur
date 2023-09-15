#include "Anggur/Log.h"

namespace Anggur {
	void log(const std::string& message) {
		static std::ofstream output("log.txt");
		output << message << "\n";
	}
}
