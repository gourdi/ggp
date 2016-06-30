#ifndef __GGO_CIRCLES_ANIMATION_ARTIST__
#define __GGO_CIRCLES_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_array.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>

class ggo_circles_animation_artist : public ggo_dynamic_background_animation_artist_abc
{
public:

        ggo_circles_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
  void	init_bkgd_buffer(uint8_t * bkgd_buffer) override;
  bool	render_next_frame_bkgd(uint8_t * output_buffer, uint8_t * bkgd_buffer, int frame_index) override;
			
private:

  struct ggo_circle_animate : public ggo_position_animate_abc
  {
          ggo_circle_animate(const ggo::pos2f & pos, int start_offset) : ggo_position_animate_abc(pos, start_offset) {};
    
    bool  update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;

    float       _radius;
    ggo::color  _color;
    float       _attenuation_factor;
    float       _bounding_factor;
      
    static  bool bkgd_rendering_allowed;
  };
    
  ggo_animator  _animator;
};

#endif
