#ifndef __GGO_FILL_BUFFER__
#define __GGO_FILL_BUFFER__

#include <stdint.h>
#include <ggo_curve.h>
#include <ggo_color.h>
#include <ggo_brush_abc.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_image_abc.h>

// Uncomment to enable 'ggo_fill_brush' functions.
// Disabled for now since it makes clang crash.
#define GGO_FILL_BRUSH 1

namespace ggo
{
  void	fill_solid(ggo::gray_image_abc & image, float gray);
  void	fill_solid(ggo::rgb_image_abc & image, const ggo::color & color);
  void	fill_solid_rgb(uint8_t * buffer, int count, const ggo::color & color);
  void	fill_solid_rgb(float * buffer, int count, const ggo::color & color);

  void	fill_checker(ggo::gray_image_abc & image, float gray1, float gray2, int tile_size);
  void	fill_checker(ggo::rgb_image_abc & image, const ggo::color & color1, const ggo::color & color2, int tile_size);

  void	fill_color_curve(ggo::gray_image_abc & image, const ggo::curve_abc<float, float> & color_curve);
  void	fill_color_curve(ggo::rgb_image_abc & image, const ggo::curve_abc<float, ggo::color> & color_curve);

  void	fill_gaussian(ggo::gray_image_abc & image,
                      float var,
                      float gray1,
                      float gray2);
  void	fill_gaussian(ggo::rgb_image_abc & image,
                      float var,
                      const ggo::color & color1,
                      const ggo::color & color2);
                      
  void	fill_4_colors(ggo::gray_image_abc & image,
                      float gray1,
                      float gray2,
                      float gray3,
                      float gray4);
  void	fill_4_colors(ggo::rgb_image_abc & image,
                      const ggo::color & color1,
                      const ggo::color & color2,
                      const ggo::color & color3,
                      const ggo::color & color4);

  #ifdef GGO_FILL_BRUSH
  void  fill_brush(ggo::gray_image_abc & image,
                   const ggo::gray_brush_abc & brush,
                   const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());                            
  void  fill_brush(ggo::rgb_image_abc & image,
                   const ggo::rgb_brush_abc & brush,
                   const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());
  #endif

  void	fill_perlin(ggo::gray_image_abc & image,
                    float size,
                    float gray1,
                    float gray2);
  void	fill_perlin(ggo::rgb_image_abc & image,
                    float size,
                    const ggo::color & color1,
                    const ggo::color & color2);
}

#endif
