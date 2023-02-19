#pragma once

#include <string>
#include "ID.h"

namespace Anggur
{
	struct Event
	{
        ID type = 0;
		bool consumed = false;

        Event(ID newType): type(newType)
        {
        }
	};
}
