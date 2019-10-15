#include "ggo_rescale_processing.h"
#include <2d/processing/ggo_scaling.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::image rescale_processing::process(ggo::image img, const parameters & params) const
  {
    ggo::size output_size = parse_scaling(params, "size", img.size());

    return scale(img, output_size, ggo::scaling_algo::cubic_integration);
  }
}