#ifndef __GGO_LAYER__
#define __GGO_LAYER__

#include <ggo_shapes2d.h>
#include <ggo_multi_scale_paint.h>
#include <ggo_sampling_paint.h>
#include <ggo_pixel_buffer.h>
#include <ggo_pixel_rect.h>

namespace ggo
{
  // Brushes.
  template <typename color_t>
  struct dyn_brush_abc
  {
    virtual color_t get(int x, int y) const = 0;
  };

  template <typename color_t>
  struct solid_dyn_brush : public dyn_brush_abc<color_t>
  {
    const color_t _brush_color;

    solid_dyn_brush(const color_t & c) : _brush_color(c) {}

    color_t get(int x, int y) const override {
      return _brush_color;
    }
  };

  // Blenders.
  template <typename output_color_t, typename brush_color_t>
  struct dyn_blender_abc
  {
    virtual output_color_t blend(int x, int y, const output_color_t & bkgd_color, const brush_color_t & brush_color) const = 0;
  };

  template <typename output_color_t, typename brush_color_t>
  struct overwrite_dyn_blender : public dyn_blender_abc<output_color_t, brush_color_t>
  {
    output_color_t blend(int x, int y, const output_color_t & bkgd_color, const brush_color_t & brush_color) const override {
      return ggo::convert_color_to<output_color_t>(brush_color);
    }
  };

  // Dynamic paint shape.
  template <typename real_type, typename output_color_t, typename brush_color_t>
  struct dyn_paint_shape
  {
    using real_t = real_type;
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point value");

    std::shared_ptr<paintable_shape2d_abc<real_type>> _shape;
    std::shared_ptr<dyn_brush_abc<brush_color_t>> _brush;
    std::shared_ptr<dyn_blender_abc<output_color_t, brush_color_t>> _blender;

    rect_data<real_t> get_bounding_rect() const { return _shape->get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<real_t> & rect_data) const { return _shape->get_rect_intersection(rect_data); }
    bool              is_point_inside(real_t x_f, real_t y_f) const { return _shape->is_point_inside(x_f, y_f); }

    brush_color_t  brush(int x, int y) const { return _brush->get(x, y); }
    output_color_t blend(int x, int y, const output_color_t & bkgd_color, const brush_color_t & brush_color) const {
      return _blender->blend(x, y, bkgd_color, brush_color); }
  };

  // Solid color shape.
  template <typename shape_t, typename color_t>
  struct solid_color_shape
  {
    using real_t = typename shape_t::type;
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point value");

    shape_t _shape;
    color_t _color;

                      solid_color_shape() = default;
                      solid_color_shape(const shape_t & shape, const color_t & color) : _shape(shape), _color(color) {}
    
    rect_data<real_t> get_bounding_rect() const { return _shape.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<real_t> & rect_data) const { return _shape.get_rect_intersection(rect_data); }
    bool              is_point_inside(real_t x_f, real_t y_f) const { return _shape.is_point_inside(x_f, y_f); }

    color_t brush(int x, int y) const { return _color; }
    color_t blend(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const { return brush_color; }
  };
}

namespace ggo
{
  template <ggo::pixel_buffer_format pbf, sampling smp, typename iterator_t,
    typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void paint_shapes(void * buffer, int width, int height, int line_step, iterator_t begin_it, iterator_t end_it)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    using item_t = std::iterator_traits<iterator_t>::value_type;

    // Lambda to retrieve pixel color.
    auto read_pixel_lambda = [&](int x, int y)
    {
      return ggo::read_pixel<pbf>(buffer, x, y, height, line_step);
    };

    // Lambda to set pixel color.
    auto write_pixel_lambda = [&](int x, int y, const color_t & c)
    {
      ggo::write_pixel<pbf>(buffer, x, y, height, line_step, c);
    };

    // Lambda that paints a block of pixels without shape sampling.
    auto paint_block_lambda = [&](const ggo::pixel_rect & block_rect, 
      std::vector<const item_t *>::const_iterator begin_it,
      std::vector<const item_t *>::const_iterator end_it)
    {
      for (int y = block_rect.bottom(); y <= block_rect.top(); ++y)
      {
        uint8_t * ptr = static_cast<uint8_t *>(get_pixel_ptr<pbf>(buffer, block_rect.left(), y, height, line_step));
        for (int x = block_rect.left(); x <= block_rect.right(); ++x)
        {
          color_t pixel_color = read_pixel<pbf>(ptr);

          for (auto it = begin_it; it != end_it; ++it)
          {
            const item_t * item = *it;
            pixel_color = item->blend(x, y, pixel_color, item->brush(x, y));
          }

          ggo::write_pixel<pbf>(ptr, pixel_color);
          ptr += pixel_buffer_format_info<pbf>::pixel_byte_size;
        }
      }
    };

    paint_multi_scale<smp>(width, height, begin_it, end_it,
      scale_factor, first_scale,
      read_pixel_lambda, write_pixel_lambda, paint_block_lambda);
  }
}

#endif
