#ifndef __GGO_DISTORSION_ANIMATION_ARTIST__
#define __GGO_DISTORSION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_array.h>
#include <ggo_color.h>

namespace ggo
{
  class distorsion_animation_artist : public animation_artist_abc
  {
  public:

          distorsion_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    void  render_frame(int frame_index, void * buffer, const ggo::rect_int & clipping) const;
    void  render_last_frame(void * buffer) const;

    struct colored_stripe
    {
      ggo::color_8u _color;
      float         _x_sup;
    };

    struct animated_transform
    {
      ggo::pos2f  _center_start;
      ggo::pos2f  _center_end;
      ggo::vec2f  _disp;
      float       _variance;
    };

    struct fixed_transform
    {
      fixed_transform(const ggo::pos2f & center, const ggo::pos2f & disp, float variance)
        :
        _center(center), _disp(disp), _variance(variance) {}

      ggo::pos2f  _center;
      ggo::vec2f  _disp;
      float       _variance;
    };

    float _hue;

    std::vector<colored_stripe>::const_iterator get_stripe_at(float x) const;

    static  float                               transform(float x, float y, const std::vector<fixed_transform> & transforms);

  private:

    int                               _frame_index;
    std::vector<colored_stripe>       _stripes;
    ggo::array<animated_transform, 1> _transforms;
  };
}

#endif
