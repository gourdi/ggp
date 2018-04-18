#ifndef __GGO_HEXA_ARTIST__
#define __GGO_HEXA_ARTIST__

#include <kernel/ggo_vec.h>
#include <ggo_random_interpolator_abc.h>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>
#include <ggo_pixel_buffer_format.h>
#include <vector>

namespace ggo
{
  class renderer_abc;

  class hexa_artist
  {
  public:

                  hexa_artist(bool enable_roughness);

    basis3d_float generate_camera_basis(float progress) const;

    void          render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                         float progress, ggo::renderer_abc & renderer) const;

  public:

    struct sinusoid
    {
      float _wave_length;
      float _amplitude;
      float _angle;
      float _phase_inf;
      float _phase_sup;
    };

  private:

    bool                  _enable_roughness;
    std::vector<sinusoid> _sinusoids;
    ggo::color_32f        _side_color;
    ggo::color_32f        _top_color;
    ggo::color_32f        _back_color;
    float                 _camera_angle_start;
    float                 _camera_angle_end;
  };
}

#endif
