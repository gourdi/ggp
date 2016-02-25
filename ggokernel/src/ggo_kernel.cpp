#include "ggo_kernel.h"

#ifdef GGO_WIN
#ifdef GGO_DEBUG

#include <windows.h>

/////////////////////////////////////////////////////////////////////
void ggo_win_trace(const char * text)
{
	OutputDebugString(text);
};

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

