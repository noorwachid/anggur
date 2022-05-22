#pragma once

#include <Anggur/System/Array.h>
#include <Anggur/System/String.h>

namespace Anggur::File 
{
	Array<uint8_t> Load(const String& path);
	String LoadText(const String& path);
}
