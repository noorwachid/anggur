#include "Anggur/Graphics/Pointer.h"
#include "Anggur/Graphics/API.h"
#include <iostream>

namespace Anggur
{
	bool ConnectPointer(void* address)
	{
		#ifndef EMSCRIPTEN
		return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(address));
		#endif

		std::cout << "GL.Version " << glGetString(GL_VERSION) << "\n";

		return true;
	}
}

