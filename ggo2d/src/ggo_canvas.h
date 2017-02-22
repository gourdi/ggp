#ifndef __GGO_CANVAS__
#define __GGO_CANVAS__

#include <ggo_pixel_buffer.h>
#include <ggo_multi_shape_paint.h>

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

    void render(void * buffer, main_direction main_dir, int width, int height, int line_byte_step, pixel_buffer_format pbf) const;

    void push_shape(const dyn_paint_shape<float, ggo::color_8u, ggo::color_8u> & shape);

    static float      from_canvas_to_render(float dist, main_direction main_dir, int render_width, int render_height);
    static float      from_render_to_canvas(float dist, main_direction main_dir, int render_width, int render_height);

    static ggo::pos2f from_canvas_to_render(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height);
    static ggo::pos2f from_render_to_canvas(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height);

    static ggo::pos2i from_canvas_to_render_pixel(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height);
    static ggo::pos2f from_render_pixel_to_canvas(const ggo::pos2i & p, main_direction main_dir, int render_width, int render_height);

  private:

    std::vector<dyn_paint_shape<float, ggo::color_8u, ggo::color_8u>> _shapes;
  };
}

#endif

