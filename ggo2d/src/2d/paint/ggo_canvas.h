#pragma once

#include <2d/paint/ggo_layer.h>
#include <2d/blend/ggo_opacity_blend.h>
#include <2d/blend/ggo_alpha_blend.h>

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

    template <typename layer_t, typename... args>
    auto & make(args &&... a)
    {
      auto * layer = new layer_t(std::forward<args>(a)...);

      _layers.emplace_back(layer);

      return *layer;
    }

    template <typename shape_t, typename brush_t, typename blender_t>
    auto & make_layer(const shape_t & shape, const brush_t & brush, const blender_t & blender = blender_t())
    {
      auto * layer = new layer_t<shape_t, color_t, brush_t, blender_t>(shape, brush, blender);

      _layers.emplace_back(layer);

      return *layer;
    }

    template <typename shape_t, typename brush_color_t>
    auto & make_layer(const shape_t& shape, const brush_color_t & color)
    {
      return make_layer(shape, solid_color_brush<brush_color_t>(color), alpha_blender<color_t, brush_color_t>());
    }

    template <typename shape_t, typename brush_color_t>
    auto & make_layer(const shape_t & shape, const brush_color_t& color, float opacity)
    {
      return make_layer(shape, solid_color_brush<brush_color_t>(color), opacity_blender<color_t, alpha_blender<color_t, brush_color_t>>(opacity));
    }

    void add_layer(std::shared_ptr<const layer<color_t, scalar_t>> shape)
    {
      _layers.emplace_back(shape);
    }

    auto begin() const { return iterator(_layers.cbegin()); }
    auto end() const { return iterator(_layers.cend()); }

    void clear() { _layers.clear(); }

  private:

    std::vector<std::shared_ptr<const layer<color_t, scalar_t>>> _layers;
  };
}
