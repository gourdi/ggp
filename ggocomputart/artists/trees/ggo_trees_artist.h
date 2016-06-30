#ifndef __GGO_TREES_ARTIST__
#define __GGO_TREES_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_paint.h>
#include <ggo_layer.h>

class ggo_trees_artist : public ggo_bitmap_artist_abc
{
public:

  ggo_trees_artist(int render_width, int render_height);

private:

	struct ggo_rule
	{
		float	_scale_width;
    float	_scale_height;
		float	_angle;
		float	_offset;
	};

  struct ggo_brush : public ggo::rgb_brush_abc
  {
                ggo_brush(float hue, float dhue, float val) : _hue(hue), _dhue(dhue), _val(val), _sat(ggo::rand_float(0.5, 1)) {}
    
    ggo::color  get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override;
       
    float _hue;
    float _dhue;
    float _sat;
    float _val;
  };

  struct ggo_leaf
  {
    ggo_leaf() : _dead(false) {}
    
   	ggo::pos2f	get_top_point() const { return 0.5f * (_top_points[0] + _top_points[1]); }
	 	ggo::pos2f	get_bottom_point() const { return 0.5f * (_bottom_points[0] + _bottom_points[1]); }
	 	float				get_top_width() const { return ggo::distance(_top_points[0], _top_points[1]); }
	 	float				get_height() const { return ggo::distance(get_top_point(), get_bottom_point()); }
	 	ggo::vec2f	get_diff() const { return get_top_point() - get_bottom_point(); }
	 	float				get_angle() const { ggo::vec2f diff(get_diff()); return std::atan2(diff.get<1>(), diff.get<0>()); }
    
    ggo::pos2f  _bottom_points[2];
    ggo::pos2f  _top_points[2];
    bool				_dead;
  };
    
private:

	void		                          render_bitmap(uint8_t * buffer) override;

  void		                          blur_buffer(uint8_t * buffer, float variance) const;
            
  std::vector<ggo::polygon2d_float> create_tree(const std::vector<ggo_rule> & rules, int max_depth, float y);
  void                              render_tree(uint8_t * buffer, const std::vector<ggo::polygon2d_float> & tree_polygons, float hue, float dhue, float val) const;
};

#endif
