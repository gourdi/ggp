#include "ggo_fill.h"
#include <2d/fill/ggo_fill.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  struct fill_functor
  {
    template <ggo::image_format format>
    static void fill_solid(ggo::image & image, const std::string & color_param)
    {
      using format_traits = ggo::image_format_traits<format>;

      if constexpr(std::is_same<format_traits::color_t, ggo::color_8u>::value == true)
      {
        auto color = parse_color_8u(color_param);
        if (!color)
        {
          throw std::runtime_error("wrong solid color parameter '" + color_param + "'");
        }

        ggo::fill_solid<format>(image.data(), image.width(), image.height(), image.line_byte_step(), *color);
      }
      else
      {
        throw std::runtime_error("unsupported format when filling image");
      }
    }

    template <ggo::image_format format>
    static void call(ggo::image & image, const parameters & params)
    {
      auto color_param = params.get({ "solid", "color" });
      if (!color_param)
      {
        throw std::runtime_error("invalid or missing color parameter for fill command");
      }

      if (params.size() != 1)
      {
        throw std::runtime_error("invalid fill parameter(s)");
      }

      fill_functor::fill_solid<format>(image, *color_param);
    }
  };

  void fill(ggo::image & image, const parameters & params)
  {
    dispatch_image_format<fill_functor>(image.format(), image, params);
  }
}