#include <2d/pipeline/ggo_rescale_node.h>
#include <2d/processing/ggo_scaling.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  rescale_node::rescale_node(algo scaling_algo, ggo::size output_size)
    :
    _scaling_algo(scaling_algo),
    _output_size(output_size)
  {
  }

  /////////////////////////////////////////////////////////////////////
  image rescale_node::process(ggo::image img) const
  {
    switch (_scaling_algo)
    {
    case algo::nearest_neighbor:
      return scale_nearest_neighbor(img, _output_size);
    case algo::bilinear_interpolation:
      return scale_linear_interpolation(img, _output_size);
    case algo::bicubic_interpolation:
      return scale_cubic_interpolation(img, _output_size);
    case algo::bilinear_integration:
      return scale_linear_integration(img, _output_size);
    }

    throw std::runtime_error("invalid scaling algorithm");
    return img;
  }
}
