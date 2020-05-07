#pragma once

#include <kernel/ggo_size.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/shapes_2d/ggo_view_transform.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_canvas.h>
#include <2d/paint/ggo_layer.h>

namespace ggo
{
  template <typename data_t>
  using keyframe = std::pair<float, data_t>;

  struct layer_animation
  {
    virtual std::shared_ptr<const layer<ggo::rgb_8u>> layer_at_time(float time, view_transform<float> transform) const = 0;
  };

  struct circle_animation : public layer_animation
  {
    std::vector<keyframe<float>> _radius;
    std::vector<keyframe<ggo::pos2_f>> _center;
    std::vector<keyframe<ggo::rgba_32f>> _color;

    std::shared_ptr<const layer<ggo::rgb_8u>> layer_at_time(float time, view_transform<float> transform) const override;
  };

  struct rectangle_animation : public layer_animation
  {
    std::vector<keyframe<ggo::pos2_f>> _p1;
    std::vector<keyframe<ggo::pos2_f>> _p2;
    std::vector<keyframe<ggo::rgba_32f>> _color;

    std::shared_ptr<const layer<ggo::rgb_8u>> layer_at_time(float time, view_transform<float> transform) const override;
  };

  struct animation
  {
    ggo::rgb_32f _bkgd_color;
    std::vector<std::unique_ptr<layer_animation>> _layer_animations;
    ggo::rect_f _view;
  };

  canvas<ggo::rgb_8u> canvas_at_time(const animation& anim, float time, size render_size);



  animation load_synfig_animation();


}
