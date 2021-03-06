#include "ggo_voronoi_bitmap_artist.h"
#include <map>
#include <kernel/trees/ggo_tree.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/signal_processing/ggo_morphology.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_blend.h>

namespace
{
  const int point_count = 500;

  struct buffer_view
  {
    uint8_t * _ptr;
    int _view_width;
    int _view_height;
    int _line_byte_step;

    uint8_t operator()(int x, int y) const { return _ptr[y * _line_byte_step + x]; }
    void    operator()(int x, int y, uint8_t v) { _ptr[y * _line_byte_step + x] = v; }
  };

  struct const_buffer_view
  {
    const uint8_t * _ptr;
    int _view_width;
    int _view_height;
    int _line_byte_step;

    uint8_t operator()(int x, int y) const { return _ptr[y * _line_byte_step + x]; }
  };

  struct voronoi_node
  {
    const ggo::tree<voronoi_node> * _parent_node;
    ggo::pos2_f                      _position;
    ggo::rgb_8u                     _color;
  };

  //////////////////////////////////////////////////////////////
  ggo::tree<voronoi_node> * find_voronoi_leaf(ggo::tree<voronoi_node> & voronoi_tree, float x, float y)
  {
    ggo::tree<voronoi_node> * cur_tree = &voronoi_tree;

    while (true)
    {
      if (cur_tree->is_leaf() == true)
      {
        return cur_tree;
      }

      ggo::tree<voronoi_node> * closest_subtree = nullptr;
      float hypot = std::numeric_limits<float>::max();
      for (auto & subtree : cur_tree->subtrees())
      {
        float hypot_cur = ggo::hypot(subtree.data()._position, ggo::pos2_f(x, y));
        if (hypot_cur < hypot)
        {
          hypot = hypot_cur;
          closest_subtree = &subtree;
        }
      }

      cur_tree = closest_subtree;
    }

    GGO_FAIL();
    return nullptr;
  }

  //////////////////////////////////////////////////////////////
  ggo::array<uint8_t, 2> round_mask(const ggo::array<uint8_t, 2> & input)
  {
    // First setup views.
    int x_min = std::numeric_limits<int>::max();
    int x_max = -1;
    int y_min = std::numeric_limits<int>::max();
    int y_max = -1;
    for (int y = 0; y < input.height(); ++y)
    {
      for (int x = 0; x < input.width(); ++x)
      {
        if (input(x, y) == 1)
        {
          x_min = std::min(x, x_min);
          x_max = std::max(x, x_max);
          y_min = std::min(y, y_min);
          y_max = std::max(y, y_max);
        }
      }
    }

    // For morpho operations to work properly, we must add a 1-pixel large border around the view.
    x_min = std::max(x_min - 1, 0);
    x_max = std::min(x_max + 1, input.width() - 1);
    y_min = std::max(y_min - 1, 0);
    y_max = std::min(y_max + 1, input.height() - 1);

    int view_offset = y_min * input.width() + x_min;
    int view_width = x_max - x_min + 1;
    int view_height = y_max - y_min + 1;

    ggo::array<uint8_t, 2> tmp(input.width(), input.height(), 0);
    ggo::array<uint8_t, 2> output(input.width(), input.height(), 0);

    const_buffer_view input_view{ input.data() + view_offset, view_width, view_height, input.width() };
    buffer_view tmp_view{ tmp.data() + view_offset, view_width, view_height, input.width() };
    buffer_view output_view{ output.data() + view_offset, view_width, view_height, input.width() };

    // Once the views are properly set up, then erode it and dilate it back with a smaller radius.
    int size_min = std::min(input.width(), input.height());

    ggo::morpho_disc(input_view, tmp_view, input_view._view_width, input_view._view_height, 0.003f * size_min, [](uint8_t cur, uint8_t ref) { return cur < ref; });
    ggo::morpho_disc(tmp_view, output_view, input_view._view_width, input_view._view_height, 0.002f * size_min, [](uint8_t cur, uint8_t ref) { return cur > ref; });

    return output;
  }

  //////////////////////////////////////////////////////////////
  ggo::array<float, 2> downsample(const ggo::array<uint8_t, 2> & input, int scale_factor)
  {
    const float norm = 1.f / ggo::square(scale_factor);

    ggo::array<float, 2> output(input.width() / scale_factor, input.height() / scale_factor);

    for (int y_out = 0; y_out < output.height(); ++y_out)
    {
      for (int x_out = 0; x_out < output.width(); ++x_out)
      {
        int sum = 0;

        for (int y_in = scale_factor * y_out; y_in < scale_factor * (y_out + 1); y_in++)
        {
          for (int x_in = scale_factor * x_out; x_in < scale_factor * (x_out + 1); x_in++)
          {
            sum += input(x_in, y_in);
          }
        }

        output(x_out, y_out) = sum * norm;
      }
    }

    return output;
  }

  //////////////////////////////////////////////////////////////
  ggo::tree<voronoi_node> create_voronoi_tree(int width, int height)
  {
    // Pick up 2 colors that are not too close.
    ggo::rgb_32f color1(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
    ggo::rgb_32f color2 = color1;
    float diff = 0.f;
    for (int i = 0; i < 4; ++i)
    {
      ggo::rgb_32f candidate(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>());
      float diff_cur = std::abs(candidate.r() - color1.r()) +
                       std::abs(candidate.g() - color1.g()) +
                       std::abs(candidate.b() - color1.b());
      if (diff_cur > diff)
      {
        diff = diff_cur;
        color2 = candidate;
      }
    }

    ggo::tree<voronoi_node> voronoi_tree({ nullptr,{ 0.f, 0.f }, ggo::white<ggo::rgb_8u>() });

    // First layer.
    for (int i = 0; i < 16; ++i)
    {
      voronoi_node node;
      node._parent_node = &voronoi_tree;
      node._position.x() = ggo::rand<float>(0.f, static_cast<float>(width));
      node._position.y() = ggo::rand<float>(0.f, static_cast<float>(height));
      node._color = ggo::black<ggo::rgb_8u>();

      voronoi_tree.create_leaf(node);
    }

    // Second layer. Leaves must be created after creating new points.
    std::map<ggo::tree<voronoi_node> *, std::vector<ggo::pos2_f>> subpoints;
    for (int i = 0; i < 1024; ++i)
    {
      ggo::pos2_f point(ggo::rand<float>(0.f, static_cast<float>(width)), ggo::rand<float>(0.f, static_cast<float>(height)));

      auto voronoi_leaf = find_voronoi_leaf(voronoi_tree, point.x(), point.y());

      subpoints[voronoi_leaf].push_back(point);
    }

    for (auto it = subpoints.begin(); it != subpoints.end(); ++it)
    {
      float ratio = ggo::rand<float>(0.8f, 1.0f / 0.8f);

      for (const auto & point : it->second)
      {
        float interp = ggo::rand<float>();

        voronoi_node node;
        node._parent_node = &voronoi_tree;
        node._position = point;
        node._color = ggo::convert_color_to<ggo::rgb_8u>(color1 * (interp * color1 + (1.f - interp) * color2));

        it->first->create_leaf(node);
      }
    }

    return voronoi_tree;
  }

  //////////////////////////////////////////////////////////////
  ggo::array<const ggo::tree<voronoi_node> *, 2> create_voronoi_map(ggo::tree<voronoi_node> & voronoi_tree,
                                                                    int width, int height, int scale_factor)
  {
    ggo::array<const ggo::tree<voronoi_node> *, 2> voronoi_map(width, height);

    for (int y = 0; y < voronoi_map.height(); y += scale_factor)
    {
      for (int x = 0; x < voronoi_map.width(); x += scale_factor)
      {
        const auto left_bottom = find_voronoi_leaf(voronoi_tree, static_cast<float>(x), static_cast<float>(y));
        const auto left_top = find_voronoi_leaf(voronoi_tree, static_cast<float>(x), static_cast<float>(y + scale_factor - 1));
        const auto right_bottom = find_voronoi_leaf(voronoi_tree, static_cast<float>(x + scale_factor - 1), static_cast<float>(y));
        const auto right_top = find_voronoi_leaf(voronoi_tree, static_cast<float>(x + scale_factor - 1), static_cast<float>(y + scale_factor - 1));

        if (left_bottom == left_top && left_top == right_bottom && right_bottom == right_top)
        {
          for (int sub_y = y; sub_y < y + scale_factor; ++sub_y)
          {
            for (int sub_x = x; sub_x < x + scale_factor; ++sub_x)
            {
              voronoi_map(sub_x, sub_y) = left_bottom;
            }
          }
        }
        else
        {
          for (int sub_y = y; sub_y < y + scale_factor; ++sub_y)
          {
            for (int sub_x = x; sub_x < x + scale_factor; ++sub_x)
            {
              voronoi_map(sub_x, sub_y) = find_voronoi_leaf(voronoi_tree, static_cast<float>(sub_x), static_cast<float>(sub_y));
            }
          }
        }
      }
    }

    return voronoi_map;
  }

  //////////////////////////////////////////////////////////////
  void paint_voronoi_map(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img,
                         const ggo::tree<voronoi_node> & voronoi_tree,
                         const ggo::array<const ggo::tree<voronoi_node> *, 2> & voronoi_map, 
                         int scale_factor)
  {
    auto paint_voronoi_leaf = [&](const ggo::tree<voronoi_node> & voronoi_leaf)
    {
      ggo::array<uint8_t, 2> mask(voronoi_map.width(), voronoi_map.height(), 0);

      for (int y = 0; y < mask.height(); ++y)
      {
        for (int x = 0; x < mask.width(); ++x)
        {
          if (&voronoi_leaf == voronoi_map(x, y))
          {
            mask(x, y) = 1;
          }
        }
      }

      mask = round_mask(mask);
      auto downsampled = downsample(mask, scale_factor);
      GGO_ASSERT_EQ(downsampled.width(), img.width());
      GGO_ASSERT_EQ(downsampled.height(), img.height());

      for (int y = 0; y < img.height(); ++y)
      {
        for (int x = 0; x < img.width(); ++x)
        {
          float opacity = downsampled(x, y);

          auto pixel_color = img.read_pixel(x, y);

          ggo::alpha_blender_rgb8u blender(opacity);
          pixel_color = blender(pixel_color, voronoi_leaf.data()._color);

          img.write_pixel(x, y, pixel_color);
        }
      }
    };

    voronoi_tree.visit_leaves(paint_voronoi_leaf);
  }
}

//////////////////////////////////////////////////////////////
void ggo::voronoi_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  int scale_factor = 4;

  auto voronoi_tree = create_voronoi_tree(scale_factor * width, scale_factor * height);
  auto voronoi_map = create_voronoi_map(voronoi_tree, scale_factor * width, scale_factor * height, scale_factor);

  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, { width, height }, line_byte_step);
  paint_voronoi_map(img, voronoi_tree, voronoi_map, scale_factor);
}
