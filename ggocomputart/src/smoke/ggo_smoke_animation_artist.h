#ifndef _GGO_SMOKE_ANIMATION_ARTIST__
#define _GGO_SMOKE_ANIMATION_ARTIST__

#include <kernel/memory/ggo_array.h>
#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class smoke_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          smoke_animation_artist(int width, int height, int line_step, ggo::image_format format);

     void  render_frame(void * buffer, int frame_index) override;
     int   frames_count() const override { return 600; }

  private:

    void process_sources(int frame_index);
    void velocity_self_advection();
    void make_incompressible();
    void opacity_advection();
    void diffuse();

    struct fluid_source
    {
      ggo::disc_double  _disc;
      double				    _speed;
      double				    _angle;
      double				    _angle_amplitude;
      double				    _angle_offset;
      double				    _wave_length;
      int					      _timer1;
      int					      _timer2;
      double				    _opacity;
    };

    double                      _cell_size;
    double                      _density;
    ggo::array<double, 2>	      _velocity_x1;
    ggo::array<double, 2>	      _velocity_x2;
    ggo::array<double, 2>	      _velocity_y1;
    ggo::array<double, 2>	      _velocity_y2;
    ggo::array<double, 2> *     _velocity_x_cur;
    ggo::array<double, 2> *     _velocity_x_tmp;
    ggo::array<double, 2> *     _velocity_y_cur;
    ggo::array<double, 2> *     _velocity_y_tmp;
    ggo::array<double, 2>	      _opacity1;
    ggo::array<double, 2>	      _opacity2;
    ggo::array<double, 2> *     _opacity_cur;
    ggo::array<double, 2> *     _opacity_tmp;
    ggo::array<fluid_source, 1> _sources;
    ggo::array<uint8_t, 1>			_bkgd_buffer;
    ggo::color_8u      	        _smoke_color;
  };
}

#endif
