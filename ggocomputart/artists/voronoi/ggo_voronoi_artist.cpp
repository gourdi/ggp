#include "ggo_voronoi_artist.h"
#include <map>
#include <ggo_tree.h>
#include <ggo_array.h>
#include <ggo_morphology.h>

#define GGO_POINTS_COUNT 500

namespace
{
  struct ggo_buffer_view
  {
    uint8_t * _ptr;
    int _view_width;
    int _view_height;
    int _line_step;

    uint8_t operator()(int x, int y) const { return _ptr[y * _line_step + x]; }
    void    operator()(int x, int y, uint8_t v) { _ptr[y * _line_step + x] = v; }
  };

  struct ggo_const_buffer_view
  {
    const uint8_t * _ptr;
    int _view_width;
    int _view_height;
    int _line_step;

    uint8_t operator()(int x, int y) const { return _ptr[y * _line_step + x]; }
  };

  struct ggo_voronoi_node
  {
    const ggo::tree<ggo_voronoi_node> * _parent_node;
    ggo::pos2f                  _position;
    ggo::color                          _color;
  };
}

namespace
{
  //////////////////////////////////////////////////////////////
  ggo::tree<ggo_voronoi_node> * find_voronoi_leaf(ggo::tree<ggo_voronoi_node> & voronoi_tree, float x, float y)
  {
    ggo::tree<ggo_voronoi_node> * cur_tree = &voronoi_tree;

    while (true)
    {
      if (cur_tree->is_leaf() == true)
      {
        return cur_tree;
      }

      ggo::tree<ggo_voronoi_node> * closest_subtree = nullptr;
      float hypot = std::numeric_limits<float>::max();
      for (auto & subtree : cur_tree->subtrees())
      {
        float hypot_cur = ggo::hypot(subtree.data()._position, ggo::pos2f(x, y));
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
    for (int y = 0; y < input.get_size<1>(); ++y)
    {
      for (int x = 0; x < input.get_size<0>(); ++x)
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
    x_max = std::min(x_max + 1, input.get_size<0>() - 1);
    y_min = std::max(y_min - 1, 0);
    y_max = std::min(y_max + 1, input.get_size<1>() - 1);

    int view_offset = y_min * input.get_size<0>() + x_min;
    int view_width = x_max - x_min + 1;
    int view_height = y_max - y_min + 1;

    ggo::array<uint8_t, 2> tmp(input.get_size<0>(), input.get_size<1>(), 0);
    ggo::array<uint8_t, 2> output(input.get_size<0>(), input.get_size<1>(), 0);

    ggo_const_buffer_view input_view{ input.data() + view_offset, view_width, view_height, input.get_size<0>() };
    ggo_buffer_view tmp_view{ tmp.data() + view_offset, view_width, view_height, input.get_size<0>() };
    ggo_buffer_view output_view{ output.data() + view_offset, view_width, view_height, input.get_size<0>() };

    // Once the views are properly set up, then erode it and dilate it back with a smaller radius.
    int size_min = std::min(input.get_size<0>(), input.get_size<1>());

    ggo::morpho_disc(input_view, tmp_view, input_view._view_width, input_view._view_height, 0.003f * size_min, [](uint8_t cur, uint8_t ref) { return cur < ref; });
    ggo::morpho_disc(tmp_view, output_view, input_view._view_width, input_view._view_height, 0.002f * size_min, [](uint8_t cur, uint8_t ref) { return cur > ref; });

    return output;
  }

  //////////////////////////////////////////////////////////////
  ggo::array<float, 2> downsample(const ggo::array<uint8_t, 2> & input, int scale_factor)
  {
    const float norm = 1.f / ggo::square(scale_factor);

    ggo::array<float, 2> output(input.get_size<0>() / scale_factor, input.get_size<1>() / scale_factor);

    for (int y_out = 0; y_out < output.get_size<1>(); ++y_out)
    {
      for (int x_out = 0; x_out < output.get_size<0>(); ++x_out)
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
  ggo::tree<ggo_voronoi_node> create_voronoi_tree(int width, int height)
  {
    // Pick up 2 colors that are not too close.
    ggo::color color1 = ggo::color::get_random();
    ggo::color color2 = color1;
    float diff = 0.f;
    for (int i = 0; i < 4; ++i)
    {
      ggo::color candidate = ggo::color::get_random();
      float diff_cur = std::abs(candidate.r() - color1.r()) +
                       std::abs(candidate.g() - color1.g()) +
                       std::abs(candidate.b() - color1.b());
      if (diff_cur > diff)
      {
        diff = diff_cur;
        color2 = candidate;
      }
    }

    ggo::tree<ggo_voronoi_node> voronoi_tree({ nullptr,{ 0.f, 0.f }, ggo::color::WHITE });

    // First layer.
    for (int i = 0; i < 16; ++i)
    {
      ggo_voronoi_node node;
      node._parent_node = &voronoi_tree;
      node._position.get<0>() = ggo::rand_float(0.f, ggo::to<float>(width));
      node._position.get<1>() = ggo::rand_float(0.f, ggo::to<float>(height));
      node._color = ggo::color::BLACK;

      voronoi_tree.create_leaf(node);
    }

    // Second layer. Leaves must be created after creating new points.
    std::map<ggo::tree<ggo_voronoi_node> *, std::vector<ggo::pos2f>> subpoints;
    for (int i = 0; i < 1024; ++i)
    {
      ggo::pos2f point(ggo::rand_float(0.f, ggo::to<float>(width)), ggo::rand_float(0.f, ggo::to<float>(height)));

      auto voronoi_leaf = find_voronoi_leaf(voronoi_tree, point.get<0>(), point.get<1>());

      subpoints[voronoi_leaf].push_back(point);
    }

    for (auto it = subpoints.begin(); it != subpoints.end(); ++it)
    {
      float ratio = ggo::rand_float(0.8f, 1.0f / 0.8f);

      for (const auto & point : it->second)
      {
        float interp = ggo::rand_float();

        ggo_voronoi_node node;
        node._parent_node = &voronoi_tree;
        node._position = point;
        node._color = ratio * (interp * color1 + (1.f - interp) * color2);

        it->first->create_leaf(node);
      }
    }

    return voronoi_tree;
  }

  //////////////////////////////////////////////////////////////
  ggo::array<const ggo::tree<ggo_voronoi_node> *, 2> create_voronoi_map(ggo::tree<ggo_voronoi_node> & voronoi_tree,
                                                                        int width, int height, int scale_factor)
  {
    ggo::array<const ggo::tree<ggo_voronoi_node> *, 2> voronoi_map(width, height);

    for (int y = 0; y < voronoi_map.get_size<1>(); y += scale_factor)
    {
      for (int x = 0; x < voronoi_map.get_size<0>(); x += scale_factor)
      {
        const auto left_bottom = find_voronoi_leaf(voronoi_tree, ggo::to<float>(x), ggo::to<float>(y));
        const auto left_top = find_voronoi_leaf(voronoi_tree, ggo::to<float>(x), ggo::to<float>(y + scale_factor - 1));
        const auto right_bottom = find_voronoi_leaf(voronoi_tree, ggo::to<float>(x + scale_factor - 1), ggo::to<float>(y));
        const auto right_top = find_voronoi_leaf(voronoi_tree, ggo::to<float>(x + scale_factor - 1), ggo::to<float>(y + scale_factor - 1));

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
              voronoi_map(sub_x, sub_y) = find_voronoi_leaf(voronoi_tree, ggo::to<float>(sub_x), ggo::to<float>(sub_y));
            }
          }
        }
      }
    }

    return voronoi_map;
  }

  //////////////////////////////////////////////////////////////
  void paint_voronoi_map(ggo::rgb_image_abc & image,
                         const ggo::tree<ggo_voronoi_node> & voronoi_tree,
                         const ggo::array<const ggo::tree<ggo_voronoi_node> *, 2> & voronoi_map, 
                         int scale_factor)
  {
    auto paint_voronoi_leaf = [&](const ggo::tree<ggo_voronoi_node> & voronoi_leaf)
    {
      ggo::array<uint8_t, 2> mask(voronoi_map.get_size<0>(), voronoi_map.get_size<1>(), 0);

      for (int y = 0; y < mask.get_size<1>(); ++y)
      {
        for (int x = 0; x < mask.get_size<0>(); ++x)
        {
          if (&voronoi_leaf == voronoi_map(x, y))
          {
            mask(x, y) = 1;
          }
        }
      }

      mask = round_mask(mask);
      auto downsampled = downsample(mask, scale_factor);
      GGO_ASSERT_EQ(downsampled.get_size<0>(), image.get_width());
      GGO_ASSERT_EQ(downsampled.get_size<1>(), image.get_height());

      image.for_each_pixel([&](int x, int y)
      {
        float opacity = downsampled(x, y);
        image.set(x, y, voronoi_leaf.data()._color, opacity);
      });
    };

    voronoi_tree.visit_leaves(paint_voronoi_leaf);
  }
}

//////////////////////////////////////////////////////////////
ggo_voronoi_artist::ggo_voronoi_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_voronoi_artist::render_bitmap(uint8_t * buffer)
{
  int scale_factor = 4;

  auto voronoi_tree = create_voronoi_tree(scale_factor * get_render_width(), scale_factor * get_render_height());
  auto voronoi_map = create_voronoi_map(voronoi_tree, scale_factor * get_render_width(), scale_factor * get_render_height(), scale_factor);
  
  auto image = make_image_buffer(buffer);
  paint_voronoi_map(image, voronoi_tree, voronoi_map, scale_factor);
}
