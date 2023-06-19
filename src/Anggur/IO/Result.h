#pragma once

namespace Anggur {
	enum class State {
		failed,
		inProgress,
		done,
	};

	struct Result {
		State state;
	};
}
