#include "Anggur/Core/GraphicFunction.h"
#include "Anggur/Core/Internal.h"
#include "Anggur/Core/Log.h"

namespace Anggur {
	void GraphicFunction::load() {
		bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		ANGGUR_ASSERT(result, "Failed to load graphic functions");
	}
}