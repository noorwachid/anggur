#include "Anggur/Graphics/Pointer.h"
#include "glad/glad.h"

namespace Anggur
{
	bool ConnectPointer(void* address)
	{
		return gladLoadGLLoader(reinterpret_cast<GLADloadproc>(address));
	}
}

