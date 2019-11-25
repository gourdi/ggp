#ifndef __GGO_IKEDA_ARTIST__
#define __GGO_IKEDA_ARTIST__

#include <ggo_animation_artist.h>
#include <kernel/math/ggo_harmonic_curve.h>

namespace ggo
{
  class ikeda_animation_artist : public progress_animation_artist_abc
  {
  public:

          ikeda_animation_artist(
            int width, int height, int line_byte_step, 
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    struct particle
    {
      ggo::pos2_f   _pos;
      ggo::rgb_8u  	_color;
      float				  _radius;
    };

  private:

    particle	create_seed() const;

  private:

    ggo::rgb_8u				          _bkgd_colors[4];
    ggo::harmonic_curve<float> 	_u0;
    ggo::harmonic_curve<float> 	_u1;
    ggo::harmonic_curve<float> 	_u2;
    float 						          _hue1;
    float 						          _hue2;
    float 						          _range;
    std::vector<particle> 	    _seeds;
  };
}

#endif
