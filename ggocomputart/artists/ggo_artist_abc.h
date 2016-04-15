#ifndef __GGO_ARTIST_ABC__
#define __GGO_ARTIST_ABC__

#include <ggo_kernel.h>
#include <ggo_set2.h>
#include <ggo_shapes2d.h>
#include <ggo_ease.h>
#include <ggo_color.h>
#include <ggo_rgb_image_buffer.h>
#include <string>
#include <memory>

class ggo_artist_abc
{
public:
	
	virtual		                         ~ggo_artist_abc() {};

          int		                      get_render_width() const  { return _render_width; }
          int		                      get_render_height() const { return _render_height; }
          int		                      get_render_max_size() const { return std::max(_render_width, _render_height); }
          int		                      get_render_min_size() const { return std::min(_render_width, _render_height); }
          int                         get_pixels_count() const { return _render_width * _render_height; }
                                      
  static	ggo::point2d_float	        map_fill(const ggo::point2d_float & point, float inf, float sup, int render_width, int render_height);
                                      
	static	float	                      map_fit(float value, float inf, float sup, int render_width, int render_height);
  static	ggo::point2d_float	        map_fit(const ggo::point2d_float & point, float inf, float sup, int render_width, int render_height);
  static	void	                      map_fit(ggo::rect_float & rect, float inf, float sup, int render_width, int render_height);
	static	void	                      map_fit(ggo::disc_float & disc, float inf, float sup, int render_width, int render_height);

          ggo::rgb_image_buffer_uint8 make_image_buffer(uint8_t * buffer) const { return ggo::rgb_image_buffer_uint8(_render_width, _render_height, buffer); }

protected:              

          ggo::point2d_float	        map_fill(const ggo::point2d_float & point, float inf, float sup) const { return map_fill(point, inf, sup, _render_width, _render_height); }
                    
          float	                      map_fit(float value, float inf, float sup) const { return map_fit(value, inf, sup, _render_width, _render_height); }
          ggo::point2d_float          map_fit(const ggo::point2d_float & point, float inf, float sup) const { return map_fit(point, inf, sup, _render_width, _render_height); }
          void	                      map_fit(ggo::rect_float & rect, float inf, float sup) const { map_fit(rect, inf, sup, _render_width, _render_height); }
          void	                      map_fit(ggo::disc_float & disc, float inf, float sup) const { map_fit(disc, inf, sup, _render_width, _render_height); }
			
          ggo::point2d_float 	        get_random_point(float margin = 0) const;
          ggo::point2d_float	        get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom) const;
          
          ggo::point2d_float 	        center() const;

                                      ggo_artist_abc(int render_width, int render_height);
                                        
          ggo::point2d_float          horz_mirror(const ggo::point2d_float & p) const;
          ggo::point2d_float          vert_mirror(const ggo::point2d_float & p) const;

private:

	int	_render_width;
	int	_render_height;
};

#endif
