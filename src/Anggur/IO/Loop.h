#pragma once

namespace Anggur
{
	enum class RunMode
	{
		Wait = 0,
		WaitOnce,
		NoWait,
	};

	bool Run(RunMode mode);
}
