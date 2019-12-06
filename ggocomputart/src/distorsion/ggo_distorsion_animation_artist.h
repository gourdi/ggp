#ifndef __GGO_DISTORSION_ANIMATION_ARTIST__
#define __GGO_DISTORSION_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include <distorsion/ggo_distorsion_artist.h>

namespace ggo
{
  class distorsion_animation_artist : public progress_animation_artist
  {
  public:

          distorsion_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    struct transform
    {
      transform(ggo::pos2_f center, float disp, float variance) :_center(center), _disp(disp), _variance(disp) {}

      ggo::pos2_f _center;
      float _disp;
      float _variance;
    };

    struct stripe
    {
      stripe(float x_sup, ggo::rgb_8u color) : _x_sup(x_sup), _color(color) {}

      float _x_sup;
      ggo::rgb_8u _color;
    };

    void  render_frame(void * buffer, float progress) override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_t(void * buffer, const std::vector<transform> & transforms) const;

    std::vector<ggo::distorsion_animation_artist::stripe>::const_iterator get_stripe_at(float x) const;

    static float apply_transforms(float x, float y, const std::vector<transform>& transforms);

  private:

    std::vector<stripe> _stripes;
    distorsion_artist _artist;
  };
}

#endif
