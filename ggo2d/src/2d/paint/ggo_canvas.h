#pragma once

#include <2d/paint/ggo_layer.h>

namespace ggo
{
  template <typename color_t_, typename scalar_t_ = float>
  class canvas
  {
  public:

    using color_t = color_t_;
    using scalar_t = scalar_t_;

    class iterator
    {
    public:

      iterator(typename std::vector<std::shared_ptr<const layer<color_t, scalar_t>>>::const_iterator it) : _it(it) {}

      const layer<color_t, scalar_t> & operator*() const { return *(*_it); }
      void operator++() { ++_it; }
      void operator--() { --_it; }
      bool operator==(const iterator& it) { return _it == it._it; }
      bool operator!=(const iterator& it) { return _it != it._it; }

    private:

      typename std::vector<std::shared_ptr<const layer<color_t, scalar_t>>>::const_iterator _it;
    };

    template <typename paint_shape_t, typename... args>
    auto & make(args &&... a)
    {
      auto * shape = new paint_shape_t(std::forward<args>(a)...);

      _paint_shapes.emplace_back(shape);

      return *shape;
    }

    template <typename shape_t, typename brush_t, typename blender_t = ggo::overwrite_blender<color_t>>
    auto & make_layer(const shape_t & shape, const brush_t & brush, const blender_t & blender = blender_t())
    {
      auto * layer = new layer_t<shape_t, color_t, brush_t, blender_t>(shape, brush, blender);

      _layers.emplace_back(layer);

      return *layer;
    }

    template <typename shape_t>
    auto & make_layer(const shape_t & shape, const color_t & color, float opacity)
    {
      return make_layer(shape, solid_color_brush<color_t>(color), alpha_blender<color_t>(opacity));
    }

    template <typename shape_t>
    auto & make_layer(const shape_t & shape, const color_t & color)
    {
      return make_layer(shape, solid_color_brush<color_t>(color), overwrite_blender<color_t>());
    }

    //template <typename paint_shape_t>
    //auto & add_shape(const paint_shape_t & shape)
    //{
    //  paint_shape_t * shape_copy = new paint_shape_t(shape);

    //  _paint_shapes.emplace_back(shape_copy);

    //  return *shape_copy;
    //}

    void add_layer(std::shared_ptr<const layer<color_t, scalar_t>> shape)
    {
      _layers.emplace_back(shape);
    }

    auto begin() const { return iterator(_layers.cbegin()); }
    auto end() const { return iterator(_layers.cend()); }

    void clear() { _paint_shapes.clear(); }

    //scene2d<color_t, scalar_t> intersect(ggo::rect_data rect) const
    //{
    //  scene2d<color_t, scalar_t> subscene;
    //  bool rect_inside_all_shapes = true;

    //  for (const auto & paint_shape : _paint_shapes)
    //  {
    //    ggo::rect_intersection intersection = paint_shape->get_rect_intersection(block_rect_data);

    //    switch (intersection)
    //    {
    //    case ggo::rect_intersection::disjoints:
    //      break;
    //    case ggo::rect_intersection::rect_in_shape:
    //      subscene.add_shape(paint_shape);
    //      break;
    //    case ggo::rect_intersection::partial_overlap:
    //    case ggo::rect_intersection::shape_in_rect:
    //    case ggo::rect_intersection::unknown:
    //      rect_inside_all_shapes = false;
    //      subscene.add_shape(paint_shape);
    //      break;
    //    }
    //  }
    //}

  private:

    std::vector<std::shared_ptr<const layer<color_t, scalar_t>>> _layers;
  };
}
