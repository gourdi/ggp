#ifndef __GGO_CANVAS__
#define __GGO_CANVAS__

#include <ggo_pixel_buffer.h>
#include <ggo_multi_shape_paint.h>
#include <memory>

namespace ggo
{
  class canvas
  {
  public:

    enum class main_direction
    {
      horizontal,
      vertical
    };

    struct view
    {
      view(const ggo::pos2f & center, float size, main_direction main_dir) : _center(center), _size(size), _main_direction(main_dir) {}

      ggo::pos2f      _center;
      float           _size;
      main_direction  _main_direction;
    };

    void render(void * buffer, const view & view, int width, int height, int line_byte_step, pixel_buffer_format pbf) const;

    disc_float * create_disc();
    polygon2d_float * create_polygon();

    void remove_shape(const ggo::paintable_shape2d_abc<float> * shape);

    static float      from_view_to_render(float dist, float view_size, main_direction main_dir, int render_width, int render_height);
    static float      from_render_to_view(float dist, float view_size, main_direction main_dir, int render_width, int render_height);

    static ggo::pos2f from_view_to_render(const ggo::pos2f & p, const view & view, int render_width, int render_height);
    static ggo::pos2f from_render_to_view(const ggo::pos2f & p, const view & view, int render_width, int render_height);

    static ggo::pos2i from_view_to_render_pixel(const ggo::pos2f & p, const view & view, int render_width, int render_height);
    static ggo::pos2f from_render_pixel_to_view(const ggo::pos2i & p, const view & view, int render_width, int render_height);

  private:

    class shape_abc
    {
    public:

      virtual const ggo::paintable_shape2d_abc<float> * get_shape() const = 0;
      virtual ggo::paintable_shape2d_abc<float> * create_render_shape(const view & view, int render_width, int render_height) const = 0;
    };

    class disc : public shape_abc
    {
    public:

      const ggo::paintable_shape2d_abc<float> * get_shape() const override { return &_disc; }
      ggo::paintable_shape2d_abc<float> * create_render_shape(const view & view, int render_width, int render_height) const override;

      ggo::disc_float _disc;
    };

    class polygon : public shape_abc
    {
    public:

      const ggo::paintable_shape2d_abc<float> * get_shape() const override { return &_polygon; }
      ggo::paintable_shape2d_abc<float> * create_render_shape(const view & view, int render_width, int render_height) const override;

      ggo::polygon2d_float _polygon;
    };

    std::vector<std::unique_ptr<shape_abc>> _shapes;
  };
}

#endif

