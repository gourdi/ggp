#include "ggo_blur.h"
#include <2d/processing/ggo_gaussian_blur.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  void blur(ggo::image & image, const parameters & params)
  {
    auto stddev = params.get<float>({ "stddev" });
    if (!stddev)
    {
      throw std::runtime_error("missing stddev parameter");
    }

    ggo::gaussian_blur(image, *stddev);
  }
}