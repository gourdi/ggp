//#include "ggo_demeco_animation_artist.h"
//#include <2d/processing/ggo_blit.h>
//#include <2d/processing/ggo_gaussian_blur.h>
//#include <2d/paint/ggo_paint.h>
//#include <2d/fill/ggo_fill.h>
//
////////////////////////////////////////////////////////////////
//ggo::demeco_animation_artist::demeco_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
//:
//animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
//_artist(width, height),
//_render_image({ width, height }),
//_shadow_image({ width, height })
//{
//}
//
////////////////////////////////////////////////////////////////
//void ggo::demeco_animation_artist::render_frame(void * buffer, int frame_index, float time_step, bool & finished)
//{
//  // Render demecos.
//  finished = false;
//  if (_artist.update() == true)
//  {
//    finished = true;
//    return;
//  }
//
//  _artist.render_tile(_render_image.data(), _render_image.line_byte_step(), frame_index, ggo::rect_int::from_size(_render_image.size()));
//
//  // Blit render buffer alpha channel into shadow buffer.
//  ggo::fill_solid(_shadow_image, 0xff);
//  const int shadow_dx = min_size() / 100;
//  const int shadow_dy = min_size() / 100;
//  for (int y = shadow_dy; y < height(); ++y)
//  {
//    for (int x = shadow_dx; x < width(); ++x)
//    {
//      auto pixel = _render_image.read_pixel(x, y);
//      _shadow_image.write_pixel(x, y, 0xff - pixel.a());
//    }
//  }
//
//  // Blur shadow.
//  float stddev = min_size() / 100.f;
//  gaussian_blur(_shadow_image, stddev);
//  
//  // Blit shadow and render buffers into output buffer.
//  image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());
//
//  blit(_shadow_image, img);
//  blit(_render_image, img);
//}
