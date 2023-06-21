#pragma once

namespace Anggur {
	enum class RunMode {
		wait = 0,
		waitOnce,
		noWait,
	};

	bool run(RunMode mode);
}
