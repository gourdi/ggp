#include "ggo_kernel.h"

#ifdef GGO_WIN
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

