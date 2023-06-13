#pragma once

namespace Anggur::IO
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

