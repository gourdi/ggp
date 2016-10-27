#ifndef __GGO_AMOROSI_ANIMATION_ARTIST__
#define __GGO_AMOROSI_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_random_interpolator_abc.h>
#include <ggo_pixel_rect.h>
#include <memory>
#include <array>

namespace ggo
{
  class amorosi_animation_artist : public static_background_animation_artist_abc
  {
  public:

    amorosi_animation_artist(int render_width, int render_height);

    struct opacity_point
    {
      ggo::pos2f  _pos;
      float       _opacity;
    };

  private:

    class random_angle_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override;
    };

    class random_width_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override;
    };

    class curve : public ggo::artist_abc
    {
    public:

      curve(int render_width, int render_height, const ggo::color_8u & color);

      void update();
      void paint(void * buffer) const;
           
      bool is_dead() const;

    private:

      const int                                 _lines_count;
      int                                       _counter;
      random_angle_interpolator                 _angle_interpolator;
      random_angle_interpolator                 _subangle_interpolator;
      random_width_interpolator                 _width_interpolator;
      ggo::pos2f                                _prv_pos;
      float                                     _prv_subangle;
      float                                     _prv_width;
      float                                     _speed;
      std::vector<std::array<opacity_point, 3>> _triangles;
      ggo::color_8u                             _color;
    };

  private:

    void init_sub() override;
    void init_bkgd_buffer(void * buffer) override;
    bool render_next_frame_bkgd(void * buffer, int frame_index) override;

    ggo::color_8u get_color() const;

  private:

    std::vector<std::unique_ptr<curve>> _curves;
    float                               _hue;
  };
}

#endif
