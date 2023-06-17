#pragma once

namespace Anggur
{
	enum class State
	{
		Error,
		InProgress,
		Done,
	};

	struct Result
	{
		State state;
	};
}

