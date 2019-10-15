#include "ggo_crop_processing.h"
#include <2d/processing/ggo_crop.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image crop_processing::process(ggo::image img, const parameters & params) const
  {
    ggo::rect_int crop_rect = parse_rect(params, img.size());

    return crop(img, crop_rect);
  }
}