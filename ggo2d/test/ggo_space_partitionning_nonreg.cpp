#include <cstring>
#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_basic_space_partitionning_2d.h>
#include <ggo_pixel_space_partitionning_2d.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(space_partitionning, test1)
{
  auto ggo_space_partitionning_nonreg = [&](const ggo::pixel_aligned_sampler_abc & sampler, const std::string & str)
  {
    const int WIDTH = 40;
    const int HEIGHT = 40;
      
    auto disc = std::make_shared<ggo::disc_float>(ggo::point2d_float(20.f, 20.f), 10.f);
    ggo::shapes_collection_rgb shapes_collection(disc, ggo::color::RED, 1.f);

    ggo::array_uint8 buffer1(WIDTH * HEIGHT * 3);
    buffer1.fill(0);
    ggo::rgb_image_data_uint8 image_data1(buffer1, WIDTH, HEIGHT);
    shapes_collection.paint(image_data1, sampler);
    
    ggo::array_uint8 buffer2(WIDTH * HEIGHT * 3);
    buffer2.fill(0);
    ggo::rgb_image_data_uint8 image_data2(buffer2, WIDTH, HEIGHT);
    shapes_collection.paint(image_data2, sampler, ggo::basic_space_partitionning_2d());
    
    ggo::array_uint8 buffer3(WIDTH * HEIGHT * 3);
    buffer3.fill(0);
    ggo::rgb_image_data_uint8 image_data3(buffer3, WIDTH, HEIGHT);
    shapes_collection.paint(image_data3, sampler, ggo::pixel_space_partitionning_2d());
      
#if 0
    ggo::save_bmp(std::string("sp_recursive").append(str).append(".bmp"), buffer1, WIDTH, HEIGHT);
    ggo::save_bmp(std::string("sp_basic").append(str).append(".bmp"), buffer2, WIDTH, HEIGHT);
    ggo::save_bmp(std::string("sp_pixel").append(str).append(".bmp"), buffer3, WIDTH, HEIGHT);
#endif
      
    GGO_CHECK(memcmp(buffer1, buffer2, buffer1.get_size()) == 0);
    GGO_CHECK(memcmp(buffer1, buffer3, buffer1.get_size()) == 0);
  };
  
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_1(), "1");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_2X2(), "2x2");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_4X4(), "4x4");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_8X8(), "8x8");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_16X16(), "16x16");
}

////////////////////////////////////////////////////////////////////
GGO_TEST(space_partitionning, test2)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;

  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 0);
  
  std::vector<ggo::paintable_shape_ptr> shapes;
  shapes.push_back(std::make_shared<ggo::disc_float>(25.f, 50.f, 30.f));
  shapes.push_back(std::make_shared<ggo::disc_float>(75.f, 50.f, 30.f));
  
  ggo::pixel_rect pixel_rect = ggo::pixel_rect::from_left_right_bottom_top(0, WIDTH - 1, 0, HEIGHT - 1);
  
  auto block_fct = [&](const ggo::pixel_rect & block)
  {
    int r = ggo::rand_int(0, 128);
    int g = ggo::rand_int(0, 128);
    
    block.for_each_pixel([&](int x, int y)
    {
      buffer[3 * (y * WIDTH + x) + 0] += r;
      buffer[3 * (y * WIDTH + x) + 1] += g;
    });
  };
  
  auto pixel_fct = [&](int x, int y, bool bypass_shape_sampling)
  {
    buffer[3 * (y * WIDTH + x) + 2] += 128;
  };
  
  ggo::recursive_space_partitionning_2d space_partitionning;
  space_partitionning.process_shapes(shapes, pixel_rect, block_fct, pixel_fct);
    
  ggo::save_bmp("test_space_partitionning.bmp", buffer, WIDTH, HEIGHT);
}
