#include "ggo_worms_bitmap_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include "ggo_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  class worms_bitmap_artist_t : public artist
  {
  public:

    //////////////////////////////////////////////////////////////
    worms_bitmap_artist_t(int width, int height) : artist(width, height) {}

    //////////////////////////////////////////////////////////////
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_bitmap(void * buffer, int line_byte_step) const
    {
      const int counter_max = 1500;

      ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step);
      ggo::fill_solid(img, ggo::white_8u());

      float hue1 = ggo::rand<float>();
      float hue2 = ggo::rand<float>();

      float variance_from = 0.01f * min_size();
      float variance_to = 0;

      for (int counter = 0; counter < counter_max; ++counter)
      {
        std::cout << "\rRendering spline " << counter << " out of " << counter_max << std::endl;

        ggo::spline<float> spline;

        // Build the spline.
        for (int i = 0; i < 10; ++i)
        {
          int sprite_size = ggo::round_to<int>(0.1f * min_size());
          int pos_x = ggo::rand<int>(0, width());
          int pos_y = ggo::rand<int>(0, height());

          ggo::pos2_f pt;
          pt.x() = pos_x + ggo::rand<float>(0, 0.1f * min_size());
          pt.y() = pos_y + ggo::rand<float>(0, 0.1f * min_size());
          spline.add_control_point(pt);
        }

        // Build the extended segments.
        int segments_count = 5 + 10 * counter / counter_max;
        auto points = spline.evaluate(segments_count);
        ggo::multi_shape_f extended_segments_inside;
        ggo::multi_shape_f extended_segments_border;

        for (int i = 1; i < points.size(); ++i)
        {
          const ggo::pos2_f & p1 = points[i - 1];
          const ggo::pos2_f & p2 = points[i];

          float inside_width = 0.003f * min_size();
          float border_width = 0.005f * min_size();

          auto inside_segment = std::make_shared<ggo::capsule_f>(p1, p2, inside_width);
          auto border_segment = std::make_shared<ggo::capsule_f>(p1, p2, border_width);

          extended_segments_inside.add_shape(inside_segment);
          extended_segments_border.add_shape(border_segment);
        }

        // Paint the spline.
        const ggo::rgb_8u color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<bool>() ? hue1 : hue2, ggo::rand<float>(), 1);
        ggo::paint<ggo::sampling_4x4>(img, extended_segments_border, ggo::black_8u());
        ggo::paint<ggo::sampling_4x4>(img, extended_segments_inside, color);

        // Blur the render buffer.
        float stddev = 0.02f * min_size();
        ggo::gaussian_blur(img, stddev);
      }
    }
  };
}

//////////////////////////////////////////////////////////////
void ggo::worms_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  worms_bitmap_artist_t artist(width, height);

  artist.render_bitmap<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, line_byte_step);
}
