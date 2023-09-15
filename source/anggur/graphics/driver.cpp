#include "anggur/graphics/driver.h"
#include "anggur/graphics/api.h"
#include <iostream>

namespace Anggur {
	bool loadGraphicsDriver(void* address) {
#ifndef EMSCRIPTEN
		return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(address));
#endif

		std::cout << "GL.Version " << glGetString(GL_VERSION) << "\n";

		return true;
	}
}
