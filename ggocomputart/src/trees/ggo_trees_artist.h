#ifndef __GGO_TREES_ARTIST__
#define __GGO_TREES_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class trees_artist : public bitmap_artist_abc
  {
  public:

                                  trees_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    struct ggo_rule
    {
      float	_scale_width;
      float	_scale_height;
      float	_angle;
      float	_offset;
    };

    //struct ggo_brush : public ggo::rgb_brush_abc
    //{
    //  ggo_brush(float hue, float dhue, float val) : _hue(hue), _dhue(dhue), _val(val), _sat(ggo::rand<float>(0.5, 1)) {}

    //  ggo::color  get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override;

    //  float _hue;
    //  float _dhue;
    //  float _sat;
    //  float _val;
    //};

    struct ggo_leaf
    {
      ggo_leaf() : _dead(false) {}

      ggo::pos2_f	get_top_point() const { return 0.5f * (_top_points[0] + _top_points[1]); }
      ggo::pos2_f	get_bottom_point() const { return 0.5f * (_bottom_points[0] + _bottom_points[1]); }
      float				get_top_width() const { return ggo::distance(_top_points[0], _top_points[1]); }
      float				height() const { return ggo::distance(get_top_point(), get_bottom_point()); }
      ggo::vec2_f	get_diff() const { return get_top_point() - get_bottom_point(); }
      float				get_angle() const { ggo::vec2_f diff(get_diff()); return ggo::angle(diff); }

      ggo::pos2_f _bottom_points[2];
      ggo::pos2_f _top_points[2];
      bool				_dead;
    };

    void                          render_bitmap(void * buffer) const override;

    std::vector<ggo::polygon2d_f> create_tree(const std::vector<ggo_rule> & rules, int max_depth, float y) const;
    void                          render_tree(void * buffer, const std::vector<ggo::polygon2d_f> & tree_polygons, float hue, float dhue, float val) const;
  };
}

#endif
