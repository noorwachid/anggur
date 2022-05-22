#include <Anggur/Graphics/Texture.h>
#include <Anggur/Graphics/Function.h>

namespace Anggur
{
	int Texture::GetMaxSlot() 
	{
		static int maxSlot = 0;
		if (maxSlot == 0) 
		{
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlot);
		}

		return maxSlot;
	}
}