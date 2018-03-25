#include "ggo_kernel.h"

#ifdef WIN32
#ifndef NDEBUG

#include <windows.h>

/////////////////////////////////////////////////////////////////////
void GGO_ASSERT(bool b)
{
	if (!b)
	{
		DebugBreak();
	}
}

#endif
#endif

