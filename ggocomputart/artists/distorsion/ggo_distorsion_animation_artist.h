#ifndef __GGO_DISTORSION_ANIMATION_ARTIST__
#define __GGO_DISTORSION_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_array.h>
#include <ggo_color.h>

class ggo_distorsion_animation_artist : public ggo_animation_artist_abc
{
public:

        ggo_distorsion_animation_artist(int render_width, int render_height);
            
  void	init_sub() override;
	void	render_last_frame(uint8_t * buffer);

private:

  struct ggo_colored_stripe
  {
    ggo::color  _color;
    float       _x_sup;
  };
  
  struct ggo_animated_transform
  {
    ggo::point2d_float  _center_start;
    ggo::point2d_float  _center_end;
    ggo::vector2d_float _disp;
    float               _variance;
  };
  
  struct ggo_transform
  {
    ggo_transform(const ggo::point2d_float & center, const ggo::vector2d_float & disp, float variance)
    :
    _center(center), _disp(disp), _variance(variance) {}
    
    ggo::point2d_float  _center;
    ggo::vector2d_float _disp;
    float               _variance;
  };
  
  float _hue;

private:

          bool	                                          render_next_frame_sub(uint8_t * buffer, int frame_index) override;

          std::vector<ggo_colored_stripe>::const_iterator get_stripe_at(float x) const;
  static  float                                           transform(float x, float y, const std::vector<ggo_transform> & transforms);
    
private:

  std::vector<ggo_colored_stripe>     _stripes;
  ggo::array<ggo_animated_transform>  _transforms;
};

#endif
