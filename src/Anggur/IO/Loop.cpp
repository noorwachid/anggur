#include "Anggur/IO/Loop.h"
#include "Anggur/IO/Internal.h"

namespace Anggur::IO
{
	bool Run(RunMode mode)
	{
		return uv_run(uv_default_loop(), static_cast<uv_run_mode>(mode));
	}
}

