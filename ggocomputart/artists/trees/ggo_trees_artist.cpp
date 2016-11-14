#include "ggo_trees_artist.h"
#include <iostream>
#include <ggo_tree.h>
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>

namespace
{
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

  ////////////////////////////////////////////////////////////////
  std::vector<ggo::polygon2d_float> create_tree(const ggo::bitmap_artist_abc & artist, const std::vector<ggo_rule> & rules, int max_depth, float y)
  {
    // Create tree with first leaf.        
    float height = 0.1f * artist.get_render_min_size();
    float dx = height / 15 * ggo::rand<float>(0.8f, 1.2f);
    float x = ggo::rand<float>(-0.2f * artist.get_render_width(), 1.2f * artist.get_render_width());

    ggo_leaf leaf;
    leaf._bottom_points[0] = ggo::pos2f(x - dx, y);
    leaf._bottom_points[1] = ggo::pos2f(x + dx, y);
    leaf._top_points[0] = ggo::pos2f(x - dx, y + height);
    leaf._top_points[1] = ggo::pos2f(x + dx, y + height);

    ggo::tree<ggo_leaf> tree(leaf);

    std::vector<ggo::polygon2d_float> polygons;

    // Create first polygon.        
    ggo::polygon2d_float polygon;
    polygon.add_point(artist.horz_mirror(leaf._top_points[0]));
    polygon.add_point(artist.horz_mirror(leaf._top_points[1]));
    polygon.add_point(artist.horz_mirror(leaf._bottom_points[1]));
    polygon.add_point(artist.horz_mirror(leaf._bottom_points[0]));
    polygons.push_back(polygon);

    // Create sub-leaves.
    for (int depth = 0; depth < max_depth; ++depth)
    {
      tree.visit_leaves([&](ggo::tree<ggo_leaf> & leaf_tree)
      {
        for (const auto & rule : rules)
        {
          ggo_leaf & leaf = leaf_tree.data();

          if (leaf._dead == true)
          {
            continue;
          }

          // Create new leaf.
          ggo_leaf new_leaf;

          float scale_width = rule._scale_width;
          float scale_height = rule._scale_height;
          float angle = rule._angle;
          float offset = rule._offset;

          // Randomize rule a bit.          
          switch (ggo::rand<int>(0, 16))
          {
          case 0:
            scale_width = ggo::rand<float>(0.7f, 0.9f);
            break;
          case 1:
            scale_height = ggo::rand<float>(0.7f, 0.9f);
            break;
          case 2:
            angle = ggo::rand<float>(-0.4f, 0.4f);
            break;
          case 4:
            leaf._dead = true;
            break;
          }

          float leaf_angle = leaf.get_angle() + angle;
          float leaf_height = leaf.get_height() * scale_height;
          float leaf_width = leaf.get_top_width() * scale_width;

          ggo::vec2f vert_disp = ggo::from_polar(leaf_angle, leaf_height);
          ggo::vec2f delta(vert_disp);
          delta.set_length(0.5f * leaf_width);
          delta = ggo::rotate(delta, ggo::pi<float>() / 2);

          new_leaf._bottom_points[0] = leaf._top_points[0];
          new_leaf._bottom_points[1] = leaf._top_points[1];
          new_leaf._top_points[0] = leaf.get_top_point() + vert_disp + delta;
          new_leaf._top_points[1] = leaf.get_top_point() + vert_disp - delta;

          if (std::fabs(offset - 1) < 0.01)
          {
            ggo::vec2f dy = leaf.get_top_point() - leaf.get_bottom_point();

            dy *= (1 - offset);

            new_leaf._bottom_points[0] -= dy;
            new_leaf._bottom_points[1] -= dy;
            new_leaf._top_points[0] -= dy;
            new_leaf._top_points[1] -= dy;
          }

          leaf_tree.create_leaf(new_leaf);

          // Create the matching polygon too.
          ggo::polygon2d_float polygon;
          polygon.add_point(artist.horz_mirror(new_leaf._top_points[0]));
          polygon.add_point(artist.horz_mirror(new_leaf._top_points[1]));
          polygon.add_point(artist.horz_mirror(new_leaf._bottom_points[1]));
          polygon.add_point(artist.horz_mirror(new_leaf._bottom_points[0]));
          polygons.push_back(polygon);
        }
      });
    }

    return polygons;
  }

  ////////////////////////////////////////////////////////////////
  void render_tree(const ggo::bitmap_artist_abc & artist, void * buffer, const std::vector<ggo::polygon2d_float> & tree_polygons, float hue, float dhue, float val)
  {
    ggo::multi_shape_float paint_polygons;
    ggo::multi_shape_float paint_borders;

    // Create the border extended segments.
    for (const auto & polygon : tree_polygons)
    {
      for (int i = 0; i < polygon.get_points_count(); ++i)
      {
        const ggo::pos2f & p1 = polygon.get_point(i);
        const ggo::pos2f & p2 = polygon.get_point((i + 1) % polygon.get_points_count());
        paint_borders.add_shape(std::make_shared<ggo::extended_segment<float>>(p1, p2, 0.002f * artist.get_render_min_size()));
      }

      paint_polygons.add_shape(std::make_shared<ggo::polygon2d_float>(polygon));
    }

    // Paint everything.
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, artist.get_render_width(), artist.get_render_height(), 3 * artist.get_render_width(),
      paint_borders, ggo::black<ggo::color_8u>());

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, artist.get_render_width(), artist.get_render_height(), 3 * artist.get_render_width(),
      paint_polygons, ggo::from_hsv<ggo::color_8u>(hue, dhue, val));
  }
}

//////////////////////////////////////////////////////////////
ggo::trees_artist::trees_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::trees_artist::render_bitmap(void * buffer) const
{
  memset(buffer, 0, 3 * get_render_width() * get_render_height());
    
	// Build the rules.
	std::vector<ggo_rule> rules;
    
  int rules_count = ggo::rand<int>(2, 3);

	for (int i = 0; i < rules_count; ++i)
	{
    ggo_rule rule;
		rule._scale_width	  = ggo::rand<float>(0.6f, 0.9f);
    rule._scale_height	= ggo::rand<float>(0.6f, 0.9f);
		rule._angle		      = ggo::rand<float>(-0.4f, 0.4f);
		// There must be at least one leaf starting from the top.
		rule._offset	      = i == 0 ? 1 : ggo::rand<float>(0.5f, 1);
		
		rules.push_back(rule);
	}
	
  // Build and render the trees.
  float stddev = 0.005f * get_render_min_size();
  float hue = ggo::rand<float>();
  float dhue = ggo::rand<float>(0.1f, 0.2f);
  int max_depth = ggo::rand<int>(8, 10);
  int counter = 0;
  
  for (float y = static_cast<float>(get_render_height()); y > -0.4f * get_render_height(); y -= 0.005f * get_render_height())
  {
    auto tree = create_tree(*this, rules, max_depth, y);
  
    float val = 1.f - y / float(get_render_height());
    render_tree(*this, buffer, tree, hue, dhue, val);

    if (counter % 2)
    {
      ggo::gaussian_blur2d_mirror<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), stddev);
      stddev *= 0.95f;
    }
    
    ++counter;
  }
}
