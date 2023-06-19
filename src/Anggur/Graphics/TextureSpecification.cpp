#include "Anggur/Graphics/TextureSpecification.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur {
	int TextureSpecification::getMaxSlot() {
		static int maxSlot = 0;
		if (maxSlot == 0)
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlot);

		return maxSlot;
	}
}
