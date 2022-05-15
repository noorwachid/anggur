#pragma once

#include <cstdint.h>

namespace Anggur::Network::ByteOrder {
	uint8_t hostToNetwork(uint8_t value) {
		return htonb(value);
	}

	uint16_t hostToNetwork(uint16_t value) {
		return htons(value);
	}
}
