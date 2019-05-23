#ifndef __GGO_SCALING__
#define __GGO_SCALING__

#include <kernel/ggo_size.h>
#include <2d/ggo_image.h>

namespace ggo
{
  image	scale(const image & input, ggo::size output_size);
}

#endif
