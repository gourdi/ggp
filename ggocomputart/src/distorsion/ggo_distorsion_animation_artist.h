#ifndef __GGO_DISTORSION_ANIMATION_ARTIST__
#define __GGO_DISTORSION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/memory/ggo_array.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class distorsion_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          distorsion_animation_artist(int width, int height, int line_step, ggo::image_format format);

    void  render_frame(void * buffer, int frame_index, float time_step) override;

    struct colored_stripe
    {
      ggo::rgb_8u _color;
      float       _x_sup;
    };

    struct animated_transform
    {
      ggo::pos2_f _center_start;
      ggo::pos2_f _center_end;
      ggo::vec2_f _disp;
      float       _variance;
    };

    struct fixed_transform
    {
      fixed_transform(const ggo::pos2_f & center, const ggo::pos2_f & disp, float variance)
        :
        _center(center), _disp(disp), _variance(variance) {}

      ggo::pos2_f _center;
      ggo::vec2_f _disp;
      float       _variance;
    };

    std::vector<colored_stripe>::const_iterator get_stripe_at(float x) const;

    static  float                               transform(float x, float y, const std::vector<fixed_transform> & transforms);

  private:

    float                             _hue = 0.f;
    std::vector<colored_stripe>       _stripes;
    ggo::array<animated_transform, 1> _transforms;
  };
}

#endif
