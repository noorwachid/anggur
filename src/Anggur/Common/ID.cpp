#include "Anggur/Common/ID.h"

namespace Anggur
{
	ID GenerateID()
	{
		static ID incrementer = 0;

		return ++incrementer;
	}
}
