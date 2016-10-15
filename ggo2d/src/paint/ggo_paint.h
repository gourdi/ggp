#ifndef __GGO_PAINT__
#define __GGO_PAINT__

#include <ggo_shapes2d.h>
#include <ggo_2d.h>
#include <ggo_blur_pixel_sampler.h>
#include <ggo_hard_paint.h>
#include <ggo_multi_scale_paint.h>
#include <ggo_seed_paint.h>

namespace ggo
{
  template <ggo::pixel_buffer_format pbf, sampling smp = sampling_4x4, bool multi_brush_sampling = false>
  void paint(void * buffer, int width, int height, const ggo::paintable_shape2d_abc<float> & shape, float opacity = 1.f);


  template <typename layer_it_t, ggo::pixel_buffer_format pbf, sampling smp, bool multi_brush_sampling = false>
  void paint(void * buffer, int width, int height, int stride, layer_it_t layer_begin, layer_it_t layer_end);
}


#endif
