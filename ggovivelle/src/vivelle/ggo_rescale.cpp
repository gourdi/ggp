#include "ggo_rescale.h"
#include <2d/processing/ggo_scaling.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image rescale(ggo::image & image, const parameters & params)
  {
    ggo::size output_size = parse_scaling(params, "size", image.size());

    return scale(image, output_size.width(), output_size.height());
  }
}