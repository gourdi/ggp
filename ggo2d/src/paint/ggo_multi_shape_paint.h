#ifndef __GGO_LAYER__
#define __GGO_LAYER__

#include <ggo_shapes2d.h>
#include <ggo_brush_abc.h>
#include <ggo_blender_abc.h>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  /*  template <typename color_t, typename intensity_t, typename data_t>
    struct layer_item
    {
      static_assert(std::is_arithmetic<intensity_t>::value, "unexpected intensity type");

      layer_item(std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>> shape,
        const color_t & color,
        intensity_t intensity = 1,
        std::shared_ptr<const ggo::blender_abc<color_t>> blender = std::make_shared<ggo::alpha_blender<color_t, data_t>>())
        :
        _shape(shape),
        _color_brush(std::make_shared<ggo::solid_brush<color_t, data_t>>(color)),
        _intensity_brush(std::make_shared<ggo::solid_brush<color_t, data_t>>(intensity)),
        _blender(blender)
      {}

      layer_item(std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>> shape,
        std::shared_ptr<const ggo::brush_abc<color_t>> color_brush,
        std::shared_ptr<const ggo::brush_abc<intensity_t>> intensity_brush,
        std::shared_ptr<const ggo::blender_abc<color_t>> blender = std::make_shared<ggo::alpha_blender<color_t>>())
        :
        _shape(shape),
        _color_brush(color_brush),
        _intensity_brush(intensity_brush),
        _blender(blender)
      {}

      std::shared_ptr<const ggo::paintable_shape2d_abc<data_t>>   _shape;
      std::shared_ptr<const ggo::brush_abc<color_t, data_t>>      _color_brush;
      std::shared_ptr<const ggo::brush_abc<intensity_t, data_t>>  _intensity_brush;
      std::shared_ptr<const ggo::blender_abc<color_t, data_t>>    _blender;

      // For this struct to be used with paint code, it must implement some methods.
      rect_data<data_t> get_bounding_rect() const { return _shape->get_bounding_rect(); }
      rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const { return _shape->get_bounding_rect(rect_data); }

    };*/
  /*
  template <typename shape_t, typename paint_brush_t, typename blender_t, typename real_type = shape_t::type>
  struct static_layer_item
  {
    shape_t       _shape;
    paint_brush_t _brush;
    blender_t     _blender;

    using real_t = real_type;

    rect_data<real_type>  get_bounding_rect() const { return _shape.get_bounding_rect(); }
    rect_intersection	    get_rect_intersection(const rect_data<real_type> & rect_data) const { return _shape.get_rect_intersection(rect_data); }
    bool                  is_point_inside(real_type x_f, real_type y_f) const { return _shape.is_point_inside(x_f, y_f); }

    color_t brush(int x, int y) const { return _color_brush.get(x, y); }
    color_t blend(const color_t & bkgd_color, const color_t & brush_color) const { return _blender.blend(bkgd_color, brush_color); }
  };*/

  template <typename shape_t, typename color_t, typename real_type = shape_t::type>
  struct solid_color_shape
  {
    color_t _color;
    shape_t _shape;

    using real_t = real_type;
    
    rect_data<real_type>  get_bounding_rect() const { return _shape.get_bounding_rect(); }
    rect_intersection	    get_rect_intersection(const rect_data<real_type> & rect_data) const { return _shape.get_rect_intersection(rect_data); }
    bool                  is_point_inside(real_type x_f, real_type y_f) const { return _shape.is_point_inside(x_f, y_f); }

    color_t brush(int x, int y) const { return _color; }
    color_t blend(const color_t & bkgd_color, const color_t & brush_color) const { return brush_color; }
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
    auto get_pixel_lambda = [&](int x, int y)
    {
      return ggo::get_pixel<pbf>(buffer, x, y, height, line_step);
    };

    // Lambda to set pixel color.
    auto set_pixel_lambda = [&](int x, int y, const color_t & c)
    {
      ggo::set_pixel<pbf>(buffer, x, y, height, line_step, c);
    };

    // Lambda that paints a block of pixels without shape sampling.
    auto paint_block_lambda = [&](const ggo::pixel_rect & block_rect, 
      std::vector<item_t>::const_iterator begin_it,
      std::vector<item_t>::const_iterator end_it)
    {
      for (int y = block_rect.bottom(); y <= block_rect.top(); ++y)
      {
        uint8_t * ptr = static_cast<uint8_t *>(get_pixel_ptr<pbf>(buffer, block_rect.left(), y, height, line_step));
        for (int x = block_rect.left(); x <= block_rect.right(); ++x)
        {
          color_t pixel_color = get_pixel<pbf>(ptr);

          for (auto it = begin_it; it != end_it; ++it)
          {
            pixel_color = it->blend(pixel_color, it->brush(x, y));
          }

          ggo::set_pixel<pbf>(ptr, pixel_color);
          ptr += pixel_buffer_format_info<pbf>::pixel_byte_size;
        }
      }
    };

    paint_multi_scale<smp>(width, height, begin_it, end_it,
      scale_factor, first_scale,
      get_pixel_lambda, set_pixel_lambda, paint_block_lambda);
  }
}

#endif
