#ifndef __GGO_CROP__
#define __GGO_CROP__

#include <2d/ggo_image.h>
#include <kernel/ggo_rect_int.h>

namespace ggo
{
  ggo::image crop(const ggo::image & input, const ggo::rect_int & crop);
}

#endif
