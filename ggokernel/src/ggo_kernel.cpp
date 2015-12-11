#include "ggo_kernel.h"

#if defined WIN32
#include <windows.h>

#if defined GGO_DEBUG

/////////////////////////////////////////////////////////////////////
void ggo_win32_trace(const char * text)
{
	OutputDebugString(text);
};

/////////////////////////////////////////////////////////////////////
void ggo_assert(bool b)
{
	if (!b)
	{
		DebugBreak();
	}
}

#endif

#endif

