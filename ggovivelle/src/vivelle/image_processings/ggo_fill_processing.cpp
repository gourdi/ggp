#include "ggo_fill_processing.h"
#include <2d/fill/ggo_fill.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  struct fill_functor
  {
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    static void fill_solid(ggo::image & image, const std::string & color_param)
    {
      using color_t = pixel_type_traits<pixel_type>::color_t;
      constexpr auto color_space = color_traits<color_t>::color_space;

      ggo::image_t<pixel_type, memory_lines_order> view(image.data(), image.size(), image.line_byte_step());

      if constexpr (color_space == color_space::rgb)
      {
        auto color = parse_color<color_t>(color_param);
        if (!color)
        {
          throw std::runtime_error("invalid fill color parameter");
        }

        ggo::fill_solid(view, *color);
      }
      else
      {
        throw std::runtime_error("unsupported color space for fill command");
      }
    }

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    static void call(ggo::image & image, const parameters & params)
    {
      if (params.size() != 1)
      {
        throw std::runtime_error("invalid fill parameter(s)");
      }

      auto color_param = params.get({ "solid", "color" });
      if (color_param)
      {
        fill_solid<pixel_type, memory_lines_order>(image, *color_param);
      }
      else
      {
        throw std::runtime_error("invalid or missing color parameter for fill command");
      }
    }
  };

  ggo::image fill_processing::process(ggo::image img, const parameters & params) const 
  {
    dispatch_image_format<fill_functor>(img.pixel_type(), img.memory_lines_order(), img, params);

    return img;
  }
}