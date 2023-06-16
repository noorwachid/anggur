#pragma once

namespace Anggur::IO
{
	enum class RunMode
	{
		Wait = 0,
		WaitOnce,
		NoWait,
	};

	bool Run(RunMode mode);
}
