#ifndef __GGO_MEMORY__
#define __GGO_MEMORY__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace ggo
{
  template <typename T>	void	mem_zero(T * datas, int count) 				      { memset(datas, 0, count * sizeof(T)); };
  template <typename T>	void	mem_copy(T * out, const T * in, int count) 	{ memcpy(out, in, count * sizeof(T)); };
}
  
#endif
