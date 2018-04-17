#include "ggo_vivelle_rescale.h"
#include <ggo_pbf_scaling.h>
#include <ggo_command.h>
#include <ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::pixel_buffer rescale(ggo::pixel_buffer & image, const parameters & params)
  {
    ggo::size output_size = parse_scaling(params, "size", image.size());

    return ggo_scale_2d(image, output_size.width(), output_size.height());
  }
}