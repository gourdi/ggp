#include "ggo_trees_artist.h"
#include <iostream>
#include <ggo_tree.h>
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo::color ggo_trees_artist::ggo_brush::get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const
{
  ggo::rect_float bounding_rect(shape.get_bounding_rect());
  
  float dhue = _dhue * (y - bounding_rect.bottom()) / bounding_rect.height();
  float val = 1 - (y - bounding_rect.bottom()) / bounding_rect.height();
  return ggo::color::from_hsv(std::fmod(_hue + dhue, 1.f), _sat, _val * val);
}

//////////////////////////////////////////////////////////////
ggo_trees_artist::ggo_trees_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_trees_artist::render_bitmap(uint8_t * buffer)
{
  memset(buffer, 0, 3 * get_render_width() * get_render_height());
    
	// Build the rules.
	std::vector<ggo_rule> rules;
    
  int rules_count = ggo::rand_int(2, 3);

	for (int i = 0; i < rules_count; ++i)
	{
    ggo_rule rule;
		rule._scale_width	  = ggo::rand_float(0.6f, 0.9f);
    rule._scale_height	= ggo::rand_float(0.6f, 0.9f);
		rule._angle		      = ggo::rand_float(-0.4f, 0.4f);
		// There must be at least one leaf starting from the top.
		rule._offset	      = i == 0 ? 1 : ggo::rand_float(0.5f, 1);
		
		rules.push_back(rule);
	}
	
  // Build and render the trees.
  float variance = 0.005f * get_render_min_size();
  float hue = ggo::rand_float();
  float dhue = ggo::rand_float(0.1f, 0.2f);
  int max_depth = ggo::rand_int(8, 10);
  int counter = 0;
  
  for (float y = static_cast<float>(get_render_height()); y > -0.4f * get_render_height(); y -= 0.005f * get_render_height())
  {
    auto tree = create_tree(rules, max_depth, y);
  
    float val = 1.f - y / float(get_render_height());
    render_tree(buffer, tree, hue, dhue, val);

    if (counter % 2)
    {
      blur_buffer(buffer, variance);
      variance *= 0.95f;
    }
    
    ++counter;
  }
}

//////////////////////////////////////////////////////////////
std::vector<ggo::polygon2d_float> ggo_trees_artist::create_tree(const std::vector<ggo_rule> & rules, int max_depth, float y)
{
  // Create tree with first leaf.        
  float height = 0.1f * get_render_min_size();
  float dx = height / 15 * ggo::rand_float(0.8f, 1.2f);
  float x = ggo::rand_float(-0.2f * get_render_width(), 1.2f * get_render_width());
  
  ggo_leaf leaf;
  leaf._bottom_points[0] = ggo::point2d_float(x - dx, y);
  leaf._bottom_points[1] = ggo::point2d_float(x + dx, y);
  leaf._top_points[0] = ggo::point2d_float(x - dx, y + height);
  leaf._top_points[1] = ggo::point2d_float(x + dx, y + height);
  
  ggo::tree<ggo_leaf> tree(leaf);
  
  std::vector<ggo::polygon2d_float> polygons;
  
  // Create first polygon.        
  ggo::polygon2d_float polygon;
  polygon.add_point(horz_mirror(leaf._top_points[0]));
  polygon.add_point(horz_mirror(leaf._top_points[1]));
  polygon.add_point(horz_mirror(leaf._bottom_points[1]));
  polygon.add_point(horz_mirror(leaf._bottom_points[0]));
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
        switch (ggo::rand_int(0, 16))
        {
        case 0:
        	scale_width = ggo::rand_float(0.7f, 0.9f);
        	break;
        case 1:
        	scale_height = ggo::rand_float(0.7f, 0.9f);
        	break;
        case 2:
        	angle = ggo::rand_float(-0.4f, 0.4f);
        	break;
        case 4:
        	leaf._dead = true;
        	break;
        }
      
      	float leaf_angle = leaf.get_angle() + angle;
      	float leaf_height = leaf.get_height() * scale_height;
        float leaf_width = leaf.get_top_width() * scale_width;
        
        ggo::vector2d_float vert_disp = ggo::point2d_float::from_polar(leaf_angle, leaf_height);
        ggo::vector2d_float delta(vert_disp);
        delta.set_length(0.5f * leaf_width);
        delta.rotate(ggo::PI<float>() / 2);
        
        new_leaf._bottom_points[0] = leaf._top_points[0];
        new_leaf._bottom_points[1] = leaf._top_points[1];
        new_leaf._top_points[0] = leaf.get_top_point() + vert_disp + delta;
        new_leaf._top_points[1] = leaf.get_top_point() + vert_disp - delta;
        
        if (std::fabs(offset - 1) < 0.01)
        {
          ggo::vector2d_float dy = leaf.get_top_point() - leaf.get_bottom_point();
          
          dy *= (1 - offset);
              
          new_leaf._bottom_points[0] -= dy;
          new_leaf._bottom_points[1] -= dy;
          new_leaf._top_points[0] -= dy;
          new_leaf._top_points[1] -= dy;
        }
        
        leaf_tree.create_leaf(new_leaf);
        
        // Create the matching polygon too.
        ggo::polygon2d_float polygon;
        polygon.add_point(horz_mirror(new_leaf._top_points[0]));
        polygon.add_point(horz_mirror(new_leaf._top_points[1]));
        polygon.add_point(horz_mirror(new_leaf._bottom_points[1]));
        polygon.add_point(horz_mirror(new_leaf._bottom_points[0]));
        polygons.push_back(polygon);
      }
    });
  }
  
  return polygons;
}

//////////////////////////////////////////////////////////////
void ggo_trees_artist::render_tree(uint8_t * buffer, const std::vector<ggo::polygon2d_float> & tree_polygons, float hue, float dhue, float val) const
{
  auto paint_polygons = std::make_shared<ggo::multi_shape_float>();
  auto paint_borders = std::make_shared<ggo::multi_shape_float>();

  // Create the border extended segments.
  for (const auto & polygon : tree_polygons)
  {
    for (int i = 0; i < polygon.get_points_count(); ++i)
    {
      const ggo::point2d_float & p1 = polygon.get_point(i);
      const ggo::point2d_float & p2 = polygon.get_point((i + 1) % polygon.get_points_count());
      paint_borders->add_shape(std::make_shared<ggo::extended_segment<float>>(p1, p2, 0.002f * get_render_min_size()));
    }
    
    paint_polygons->add_shape(std::make_shared<ggo::polygon2d_float>(polygon));
  }

  // Paint everything.
  ggo::paint(buffer, get_render_width(), get_render_height(), paint_borders, ggo::color::BLACK);
  ggo::paint(buffer, get_render_width(), get_render_height(), paint_polygons, ggo::color::from_hsv(hue, dhue, val));
}
    
//////////////////////////////////////////////////////////////
void ggo_trees_artist::blur_buffer(uint8_t * buffer, float variance) const
{
	ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 0, buffer + 0, get_render_width(), get_render_height(), variance, 0.001f);
	ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 1, buffer + 1, get_render_width(), get_render_height(), variance, 0.001f);
	ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 2, buffer + 2, get_render_width(), get_render_height(), variance, 0.001f);
}
