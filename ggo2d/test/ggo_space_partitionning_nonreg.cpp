#include <cstring>
#include <ggo_nonreg.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(space_partitionning, compare)
{
  auto ggo_space_partitionning_nonreg = [&](const ggo::pixel_aligned_sampler_abc & sampler, const std::string & str)
  {
    const int WIDTH = 40;
    const int HEIGHT = 40;
      
    auto disc = std::make_shared<ggo::disc_float>(ggo::point2d_float(20.f, 20.f), 10.f);
    const std::vector<ggo::rgb_layer> layers{ {disc, ggo::color::RED, 1.f} };

    ggo::rgb_image_data_uint8 image_data1(WIDTH, HEIGHT, ggo::color::BLACK);
    ggo::paint(image_data1, layers, sampler);
    
    ggo::rgb_image_data_uint8 image_data2(WIDTH, HEIGHT, ggo::color::BLACK);
    ggo::paint(image_data2, layers, sampler, ggo::space_partitionning::none);

    ggo::rgb_image_data_uint8 image_data3(WIDTH, HEIGHT, ggo::color::BLACK);
    ggo::paint(image_data3, layers, sampler, ggo::space_partitionning::block8x8);

#if 1
    ggo::save_bmp(std::string("sp_recursive_").append(str).append(".bmp"), image_data1.get_buffer(), WIDTH, HEIGHT);
    ggo::save_bmp(std::string("sp_none_").append(str).append(".bmp"), image_data2.get_buffer(), WIDTH, HEIGHT);
    ggo::save_bmp(std::string("sp_block8x8_").append(str).append(".bmp"), image_data3.get_buffer(), WIDTH, HEIGHT);
#endif
      
    GGO_CHECK(memcmp(image_data1.get_buffer(), image_data2.get_buffer(), WIDTH * HEIGHT * 3) == 0);
    GGO_CHECK(memcmp(image_data1.get_buffer(), image_data3.get_buffer(), WIDTH * HEIGHT * 3) == 0);
  };
  
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_1(), "1");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_2X2(), "2x2");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_4X4(), "4x4");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_8X8(), "8x8");
  ggo_space_partitionning_nonreg(ggo::pixel_sampler_16X16(), "16x16");
}
