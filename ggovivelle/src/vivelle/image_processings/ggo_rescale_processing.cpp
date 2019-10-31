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

    scaling_algo algo = scaling_algo::cubic_integration;
    auto algo_param = params.get({ "algo" });
    if (algo_param)
    {
      if (*algo_param == "nearest_neighbor")
      {
        algo = scaling_algo::nearest_neighbor;
      }
      else if (*algo_param == "linear_integration")
      {
        algo = scaling_algo::linear_integration;
      }
      else if (*algo_param == "cubic_integration")
      {
        algo = scaling_algo::cubic_integration;
      }
      else if (*algo_param == "linear_resampling_1")
      {
        algo = scaling_algo::linear_resampling_1;
      }
      else if (*algo_param == "linear_resampling_2x2")
      {
        algo = scaling_algo::linear_resampling_2x2;
      }
      else if (*algo_param == "linear_resampling_4x4")
      {
        algo = scaling_algo::linear_resampling_4x4;
      }
      else  if (*algo_param == "linear_resampling_16x16")
      {
        algo = scaling_algo::linear_resampling_16x16;
      }
      else if (*algo_param == "cubic_resampling_1")
      {
        algo = scaling_algo::cubic_resampling_1;
      }
      else if (*algo_param == "cubic_resampling_2x2")
      {
        algo = scaling_algo::cubic_resampling_2x2;
      }
      else if (*algo_param == "cubic_resampling_4x4")
      {
        algo = scaling_algo::cubic_resampling_4x4;
      }
      else if (*algo_param == "cubic_resampling_8x8")
      {
        algo = scaling_algo::cubic_resampling_8x8;
      }
      else if (*algo_param == "cubic_resampling_16x16")
      {
        algo = scaling_algo::cubic_resampling_16x16;
      }
      else
      {
        throw std::runtime_error("invalid scaling algorithm");
      }
    }

    return scale(img, output_size, algo);
  }
}