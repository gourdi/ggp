#ifndef __GGO_AMOROSI_ANIMATION_ARTIST__
#define __GGO_AMOROSI_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_random_interpolator_abc.h>
#include <ggo_pixel_rect.h>
#include <memory>
#include <array>

class ggo_amorosi_animation_artist : public ggo_static_background_animation_artist_abc
{
public:

  ggo_amorosi_animation_artist(int render_width, int render_height);

  struct ggo_opacity_point
  {
    ggo::pos2f  _pos;
    float       _opacity;
  };

private:

  class ggo_random_angle_interpolator : public ggo::random_interpolator_abc<float, float>
  {
    void get_random_data(float & data, float & dt) override;
  };
    
  class ggo_random_width_interpolator : public ggo::random_interpolator_abc<float, float>
  {
    void get_random_data(float & data, float & dt) override;
  };

  class ggo_curve : public ggo_artist_abc
  {
  public:
  
          ggo_curve(int render_width, int render_height, const ggo::color & color);
    
    void  update();
    void  paint(uint8_t * buffer) const;
        
    bool  is_dead() const;

  private:

    const int                                     _lines_count;
    int                                           _counter;
    ggo_random_angle_interpolator                 _angle_interpolator;
    ggo_random_angle_interpolator                 _subangle_interpolator;
    ggo_random_width_interpolator                 _width_interpolator;
    ggo::pos2f                                    _prv_pos;
    float                                         _prv_subangle;
    float                                         _prv_width;
    float                                         _speed;
    std::vector<std::array<ggo_opacity_point, 3>> _triangles;
    ggo::color                                    _color;
  };

private:

  void	      init_sub() override;
  void	      init_bkgd_buffer(uint8_t * buffer) override;
  bool	      render_next_frame_bkgd(uint8_t * buffer, int frame_index) override;
    
  ggo::color  get_color() const;
    
private:

  std::vector<std::shared_ptr<ggo_curve>> _curves;
  float                                   _hue;
};

#endif
