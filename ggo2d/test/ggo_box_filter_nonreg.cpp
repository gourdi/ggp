#include <ggo_nonreg.h>
#include <ggo_box_filter.h>
#include <ggo_array.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(box_filter, box_filter3x3)
{
  const int WIDTH = 50;
  const int HEIGHT = 60;
    
  auto disc = std::make_shared<ggo::disc_float>(0.5f * WIDTH, 0.5f * HEIGHT, 20.f);
    
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 0);
  ggo::paint(buffer, WIDTH, HEIGHT, disc, ggo::color::WHITE, 0.8f);
  
  ggo::box_filter3_2d(buffer.data() + 0, WIDTH, HEIGHT, 3);
  ggo::box_filter3_2d(buffer.data() + 1, WIDTH, HEIGHT, 3);
  ggo::box_filter3_2d(buffer.data() + 2, WIDTH, HEIGHT, 3);

  ggo::save_bmp("box_filter3x3.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(box_filter, box_filter5x5)
{
  const int WIDTH = 50;
  const int HEIGHT = 60;
    
  auto disc = std::make_shared<ggo::disc_float>(0.5f * WIDTH, 0.5f * HEIGHT, 20.f);
    
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 0);
  ggo::paint(buffer, WIDTH, HEIGHT, disc, ggo::color::WHITE, 0.8f);
  
  ggo::box_filter5_2d(buffer.data() + 0, WIDTH, HEIGHT, 3);
  ggo::box_filter5_2d(buffer.data() + 1, WIDTH, HEIGHT, 3);
  ggo::box_filter5_2d(buffer.data() + 2, WIDTH, HEIGHT, 3);

  ggo::save_bmp("box_filter5x5.bmp", buffer, WIDTH, HEIGHT);
}
