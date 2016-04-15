#ifndef __GGO_SEED_PAINT__
#define __GGO_SEED_PAINT__

#include <stdint.h>
#include <ggo_image_abc.h>
#include <ggo_blender_abc.h>
#include <ggo_pixel_sampler_abc.h>

namespace ggo
{
  void	paint_seed_shape(ggo::rgb_image_abc & image,
                         const ggo::seed_paintable_shape2d_abc<float> & shape,
                         const ggo::color & color,
                         float opacity = 1,
                         const ggo::pixel_aligned_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
                         const ggo::rgb_blender_abc & blender = ggo::rgb_alpha_blender());

  void	paint_seed_shape(ggo::gray_image_abc & image,
                         const ggo::seed_paintable_shape2d_abc<float> & shape,
                         float gray,
                         float opacity = 1,
                         const ggo::pixel_aligned_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
                         const ggo::gray_blender_abc & blender = ggo::gray_alpha_blender());
}

#endif
