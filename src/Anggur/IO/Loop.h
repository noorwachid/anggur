#pragma once

namespace Anggur::IO
{
	enum class RunMode
	{
		Wait = 0,
		Once,
		NoWait,
	};

	bool Run(RunMode mode);
}
