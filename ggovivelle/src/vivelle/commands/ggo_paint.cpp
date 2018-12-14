#include "ggo_paint.h"
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/paint/ggo_paint.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  std::unique_ptr<const ggo::paintable_shape2d_abc<float>> create_disc(const std::string & disc_str)
  {
    auto params = split(disc_str, ',');
    if (params.size() != 3)
    {
      throw std::runtime_error("failed parsing disc parameters: '" + disc_str + "'");
    }

    float x = ggo::to<float>(params[0]);
    float y = ggo::to<float>(params[1]);
    float radius = ggo::to<float>(params[2]);

    return std::make_unique<const ggo::disc_float>(ggo::pos2f(x, y), radius);
  }

  std::unique_ptr<const ggo::paintable_shape2d_abc<float>> get_shape(const parameters & params)
  {
    auto disc_str = params.get({ "disc" });
    if (disc_str)
    {
      return create_disc(*disc_str);
    }

    throw std::runtime_error("missing shape");
    return nullptr;
  }

  struct paint_functor
  {
    template <ggo::image_format format>
    static void call(ggo::image & image, std::unique_ptr<const ggo::paintable_shape2d_abc<float>> shape, const parameters & params)
    {
      using format_traits = ggo::image_format_traits<format>;

      auto color_param = params.get({ "solid", "color" });
      if (!color_param)
      {
        throw std::runtime_error("invalid or missing color parameter for paint command");
      }

      if constexpr(std::is_same<format_traits::color_t, ggo::rgb_8u>::value == true)
      {
        auto color = parse_color_8u(*color_param);
        if (!color)
        {
          throw std::runtime_error("wrong solid color parameter '" + *color_param + "'");
        }

        ggo::paint<format, ggo::sampling_8x8>(image.data(), image.width(), image.height(), image.line_byte_step(), *shape, *color);
      }
      else
      {
        throw std::runtime_error("unsupported format when filling image");
      }
    }
  };

  void paint(ggo::image & image, const parameters & params)
  {
    auto shape = get_shape(params);

    dispatch_image_format<paint_functor>(image.format(), image, std::move(shape), params);
  }
}

