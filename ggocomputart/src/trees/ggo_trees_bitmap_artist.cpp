#include "ggo_trees_bitmap_artist.h"
#include <iostream>
#include <kernel/trees/ggo_tree.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include "ggo_artist.h"

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

  class trees_artist_t : public ggo::artist
  {
  public:

    //////////////////////////////////////////////////////////////
    trees_artist_t(int width, int height) : artist(width, height) {}

    //////////////////////////////////////////////////////////////
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_bitmap(void * buffer, int line_byte_step)
    {
      ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step);

      ggo::fill_black(img);

      // Build the rules.
      std::vector<ggo_rule> rules;

      int rules_count = ggo::rand<int>(2, 3);

      for (int i = 0; i < rules_count; ++i)
      {
        ggo_rule rule;
        rule._scale_width = ggo::rand<float>(0.6f, 0.9f);
        rule._scale_height = ggo::rand<float>(0.6f, 0.9f);
        rule._angle = ggo::rand<float>(-0.4f, 0.4f);
        // There must be at least one leaf starting from the top.
        rule._offset = i == 0 ? 1 : ggo::rand<float>(0.5f, 1);

        rules.push_back(rule);
      }

      // Build and render the trees.
      float stddev = 0.005f * min_size();
      float hue = ggo::rand<float>();
      float dhue = ggo::rand<float>(0.1f, 0.2f);
      int max_depth = ggo::rand<int>(8, 10);
      int counter = 0;

      for (float y = static_cast<float>(height()); y > -0.4f * height(); y -= 0.005f * height())
      {
        auto tree = create_tree(rules, max_depth, y);

        float val = 1.f - y / float(height());
        render_tree(buffer, line_byte_step, tree, hue, dhue, val);

        if (counter % 2)
        {
          ggo::gaussian_blur(img, stddev);
          stddev *= 0.95f;
        }

        ++counter;
      }
    }

    //////////////////////////////////////////////////////////////
    std::vector<ggo::polygon2d_f> create_tree(const std::vector<ggo_rule> & rules, int max_depth, float y) const
    {
      // Create tree with first leaf.        
      float height = 0.1f * min_size();
      float dx = height / 15 * ggo::rand<float>(0.8f, 1.2f);
      float x = ggo::rand<float>(-0.2f * width(), 1.2f * width());

      ggo_leaf leaf;
      leaf._bottom_points[0] = ggo::pos2_f(x - dx, y);
      leaf._bottom_points[1] = ggo::pos2_f(x + dx, y);
      leaf._top_points[0] = ggo::pos2_f(x - dx, y + height);
      leaf._top_points[1] = ggo::pos2_f(x + dx, y + height);

      ggo::tree<ggo_leaf> tree(leaf);

      std::vector<ggo::polygon2d_f> polygons;

      // Create first polygon.        
      ggo::polygon2d_f polygon;
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
            float leaf_height = leaf.height() * scale_height;
            float leaf_width = leaf.get_top_width() * scale_width;

            ggo::vec2_f vert_disp = ggo::vec2_f::from_angle(leaf_angle) * leaf_height;
            ggo::vec2_f delta = normalize(vert_disp) * (0.5f * leaf_width);
            delta = ggo::rotate(delta, ggo::pi<float>() / 2);

            new_leaf._bottom_points[0] = leaf._top_points[0];
            new_leaf._bottom_points[1] = leaf._top_points[1];
            new_leaf._top_points[0] = leaf.get_top_point() + vert_disp + delta;
            new_leaf._top_points[1] = leaf.get_top_point() + vert_disp - delta;

            if (std::fabs(offset - 1) < 0.01)
            {
              ggo::vec2_f dy = leaf.get_top_point() - leaf.get_bottom_point();

              dy *= (1 - offset);

              new_leaf._bottom_points[0] -= dy;
              new_leaf._bottom_points[1] -= dy;
              new_leaf._top_points[0] -= dy;
              new_leaf._top_points[1] -= dy;
            }

            leaf_tree.create_leaf(new_leaf);

            // Create the matching polygon too.
            ggo::polygon2d_f polygon;
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
    void render_tree(void * buffer, int line_byte_step, const std::vector<ggo::polygon2d_f> & tree_polygons, float hue, float dhue, float val) const
    {
      ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step);

      ggo::multi_shape_f paint_polygons;
      ggo::multi_shape_f paint_borders;

      // Create the border extended segments.
      for (const auto & polygon : tree_polygons)
      {
        for (int i = 0; i < polygon.get_points_count(); ++i)
        {
          const ggo::pos2_f & p1 = polygon.get_point(i);
          const ggo::pos2_f & p2 = polygon.get_point((i + 1) % polygon.get_points_count());
          paint_borders.add_shape(std::make_shared<ggo::capsule_f>(p1, p2, 0.002f * min_size()));
        }

        paint_polygons.add_shape(std::make_shared<ggo::polygon2d_f>(polygon));
      }

      // Paint everything.
      ggo::paint<ggo::sampling_4x4>(img, paint_borders, ggo::black_8u());
      ggo::paint<ggo::sampling_4x4>(img, paint_polygons, ggo::from_hsv<ggo::rgb_8u>(hue, dhue, val));
    }
  };
}

////////////////////////////////////////////////////////////////
void ggo::trees_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  trees_artist_t artist(width, height);

  artist.render_bitmap<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, line_byte_step);
}
