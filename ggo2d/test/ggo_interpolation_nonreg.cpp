#include <array>
#include <ggo_nonreg.h>
#include <ggo_data_fetcher.h>
#include <ggo_interpolation2d.h>
#include <ggo_rgb_image_data.h>
#include <ggo_bmp.h>

namespace
{
  ////////////////////////////////////////////////////////////////////
  void test_interpolation(std::function<float(const uint8_t * input, int width, int height, float x, float y, const ggo::data_fetcher2d_abc<uint8_t> & data_fetcher)> interpolate_func, 
                                              const std::string & filename,
                                              const ggo::data_fetcher2d_abc<uint8_t> & data_fetcher)
  {
    const int SIZE = 800;
  
    ggo::rgb_image_data_uint8 image_data(SIZE, SIZE);
    
    const std::array<uint8_t, 9> input {{50, 25, 185, 200, 132, 98, 43, 77, 210}};
    
    for (int y = 0; y < SIZE; ++y)
    {
      for (int x = 0; x < SIZE; ++x)
      {
        float x_f = 0.025f * x - 4;
        float y_f = 0.025f * y - 4;
        
        float v = ggo::to<uint8_t>(interpolate_func(input.data(), 3, 3, x_f, y_f, data_fetcher));
        
        image_data.pack(x, y, ggo::color(v / 255.f));
      }
    }
    
    ggo::save_bmp(filename, image_data.get_buffer(), SIZE, SIZE);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation, test)
{
  auto bilinear_interpolation_func = [](const uint8_t * input, int width, int height, float x, float y, const ggo::data_fetcher2d_abc<uint8_t> & data_fetcher)
  {
    return ggo::bilinear_interpolation2d<uint8_t, float>(input, width, height, x, y, 1, data_fetcher);
  };
  
  auto bicubic_interpolation_func = [](const uint8_t * input, int width, int height, float x, float y, const ggo::data_fetcher2d_abc<uint8_t> & data_fetcher)
  {
    return ggo::bicubic_interpolation2d<uint8_t, float>(input, width, height, x, y, 1, data_fetcher);
  };
  
  ggo::duplicated_edge_mirror_data_fetcher2d<uint8_t> duplicated_edge_mirror_data_fetcher;
  ggo::fixed_value_data_fetcher2d<uint8_t> fixed_value_data_fetcher(0);
  
  test_interpolation(bilinear_interpolation_func, "test_bilinear_mirror_duplicate_edge.bmp", duplicated_edge_mirror_data_fetcher);
  test_interpolation(bicubic_interpolation_func, "test_bicubic_mirror_duplicate_edge.bmp", duplicated_edge_mirror_data_fetcher);
  test_interpolation(bilinear_interpolation_func, "test_bilinear_fixed_edge.bmp", fixed_value_data_fetcher);
  test_interpolation(bicubic_interpolation_func, "test_bicubic_mirror_fixed_edge.bmp", fixed_value_data_fetcher);
}