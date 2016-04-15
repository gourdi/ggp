#ifndef __GGO_PAINT_HELPERS__
#define __GGO_PAINT_HELPERS__

#include <stdint.h>
#include <ggo_color.h>
#include <ggo_gray_image_data.h>
#include <ggo_rgb_image_data.h>
#include <ggo_color.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_blender_abc.h>

// Dots
namespace ggo
{
  void	paint_dot_rgb(uint8_t * buffer, int width, int height,
                      float x, float y, float radius,
                      const ggo::color & color, float opacity = 1);
  void	paint_dot(ggo::rgb_image_buffer_abc & image_data,
                  float x, float y, float radius,
                  const ggo::color & color,
                  float opacity = 1,
                  const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
                  const ggo::rgb_blender_abc & blender = ggo::rgb_alpha_blender());
  void	paint_dot(ggo::gray_image_buffer_abc & image_data,
                  float x, float y, float radius,
                  float gray,
                  float opacity = 1,
                  const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
                  const ggo::gray_blender_abc & blender = ggo::gray_alpha_blender());
}

#endif
