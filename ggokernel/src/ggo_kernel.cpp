#include "ggo_kernel.h"

#ifdef WIN32
#ifdef GGO_DEBUG

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

