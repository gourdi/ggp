#include "ggo_crop.h"
#include <2d/processing/ggo_crop.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image crop(ggo::image & image, const parameters & params)
  {
    ggo::rect_int crop_rect = parse_rect(params, image.size());

    return crop(image, crop_rect);
  }
}