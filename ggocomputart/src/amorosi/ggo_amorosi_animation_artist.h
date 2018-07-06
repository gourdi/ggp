#ifndef __GGO_AMOROSI_ANIMATION_ARTIST__
#define __GGO_AMOROSI_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <kernel/ggo_random_interpolator_abc.h>
#include <memory>
#include <array>

namespace ggo
{
  class amorosi_animation_artist : public animation_artist_abc
  {
  public:

    amorosi_animation_artist(int width, int height, int line_step, ggo::image_format format);

    struct opacity_point
    {
      ggo::pos2f  _pos;
      float       _opacity = 0.f;
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

    class curve : public ggo::artist
    {
    public:

            curve(int width, int height, int line_step, ggo::image_format format, const ggo::color_32f & color);

      void  update();
      void  paint(void * buffer) const;
            
      bool  is_dead() const;

    private:

      const int                                 _lines_count;
      int                                       _counter = 0;
      random_angle_interpolator                 _angle_interpolator;
      random_angle_interpolator                 _subangle_interpolator;
      random_width_interpolator                 _width_interpolator;
      ggo::pos2f                                _prv_pos;
      float                                     _prv_subangle = 0.f;
      float                                     _prv_width = 0.f;
      float                                     _speed = 0.f;
      std::vector<std::array<opacity_point, 3>> _triangles;
      ggo::color_32f                            _color;
    };

  private:

    void render_frame(void * buffer, int frame_index, bool & finished) override;

    ggo::color_32f get_color() const;

  private:

    std::vector<std::unique_ptr<curve>> _curves;
    float                               _hue;
  };
}

#endif
