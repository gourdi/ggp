#include "ggo_vivelle_rescale.h"
#include <processing/ggo_scaling.h>
#include <ggo_command.h>
#include <ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image rescale(ggo::image & image, const parameters & params)
  {
    ggo::size output_size = parse_scaling(params, "size", image.size());

    return scale(image, output_size.width(), output_size.height());
  }
}