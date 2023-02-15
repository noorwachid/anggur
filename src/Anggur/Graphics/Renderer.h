#pragma once

#include "CanvasRenderer.h"
#include "MeshRenderer.h"

namespace Anggur
{
	class Renderer
	{
	  public:
		CanvasRenderer canvas;
		MeshRenderer mesh;
	};
}
