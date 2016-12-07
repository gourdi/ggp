#ifndef __GGO_IKEDA_ARTIST__
#define __GGO_IKEDA_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_harmonic_curve.h>

namespace ggo
{
  class ikeda_artist : public animation_artist_abc
  {
  public:

          ikeda_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    struct particle
    {
      ggo::pos2f    _pos;
      ggo::color_8u	_color;
      float				  _radius;
    };

  private:

    particle	create_seed() const;

  private:

    int                         _frame_index;
    ggo::color_8u				        _bkgd_colors[4];
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
