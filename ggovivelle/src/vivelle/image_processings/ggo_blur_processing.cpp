#include "ggo_blur_processing.h"
#include <2d/processing/ggo_gaussian_blur.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  ggo::image blur_processing::process(ggo::image img, const parameters & params) const
  {
    auto stddev = params.get<float>({ "stddev" });
    if (!stddev)
    {
      throw std::runtime_error("missing stddev parameter");
    }

    ggo::gaussian_blur(img, *stddev);

    return img;
  }
}