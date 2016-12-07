#ifndef _GGO_SMOKE_ANIMATION_ARTIST__
#define _GGO_SMOKE_ANIMATION_ARTIST__

#include <ggo_array.h>
#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class smoke_animation_artist : public animation_artist_abc
  {
  public:

          smoke_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    class loop_array2d : public ggo::array<double, 2>
    {
    public:

              loop_array2d(int size_x, int size_y);

      double 	interpolate(double x, double y) const;
      double 	loop_value(int x, int y) const;
    };

    struct fluid_source
    {
      ggo::circle_double	_circle;
      double				      _speed;
      double				      _angle;
      double				      _angle_amplitude;
      double				      _angle_offset;
      double				      _wave_length;
      int					        _timer1;
      int					        _timer2;
      double				      _density;
    };

    int                         _frame_index;
    loop_array2d	              _velocity_x1;
    loop_array2d	              _velocity_x2;
    loop_array2d	              _velocity_y1;
    loop_array2d	              _velocity_y2;
    loop_array2d *              _velocity_x_cur;
    loop_array2d *              _velocity_x_tmp;
    loop_array2d *              _velocity_y_cur;
    loop_array2d *              _velocity_y_tmp;
    loop_array2d	              _density1;
    loop_array2d	              _density2;
    loop_array2d *              _density_cur;
    loop_array2d *              _density_tmp;
    ggo::array<fluid_source, 1> _sources;
    ggo::array<uint8_t, 1>			_bkgd_buffer;
    ggo::color_8u      	        _smoke_color;
  };
}

#endif
