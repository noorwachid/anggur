#pragma once

#include "Anggur/Common/Integral.h"

namespace Anggur
{
	using ID = usize;

	ID GenerateID();

	template <typename T> ID GetID()
	{
		static ID id = GenerateID();

		return id;
	}
}
