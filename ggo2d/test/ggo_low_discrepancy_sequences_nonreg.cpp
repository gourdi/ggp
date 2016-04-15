#include <sstream>
#include <array>
#include <ggo_nonreg.h>
#include <ggo_halton.h>
#include <ggo_best_candidate_sequence.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_paint.h>
#include <ggo_bmp.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  void save_points(const std::vector<ggo::point2d_float> & points, int points_count, const std::string & filename)
  {
    const int SIZE = 400;
    
    ggo::rgb_image_buffer_uint8 image_data(SIZE, SIZE);
    image_data.fill(ggo::color::BLACK);

    for (int i = 0; i < points_count; ++i)
    {
      ggo::paint(image_data, std::make_shared<ggo::disc_float>(SIZE * points[i].x(), SIZE * (1 - points[i].y()), 2.f), ggo::color::WHITE);
    }
    
    ggo::save_bmp(filename, image_data.get_buffer(), image_data.get_width(), image_data.get_height());
  }
  
  /////////////////////////////////////////////////////////////////////  
  void test_halton(int base1, int base2)
  {
    auto points = ggo::halton_2d(base1, base2, 512);

    std::array<int, 5> counts {32, 64, 128, 256, 512};
    
    for (const auto count : counts)
    {
      std::ostringstream oss;
      oss << "halton" << base1 << 'x' << base2 << "_" << count << ".bmp";
      save_points(points, count, oss.str());
    }
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(low_discrepancy_sequences, halton)
{
  test_halton(2, 3);
  test_halton(2, 5);
  test_halton(3, 5);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(low_discrepancy_sequences, best_candidate)
{
  auto points = ggo::best_candidate_sequence_2d(512);

  std::array<int, 5> counts {32, 64, 128, 256, 512};
  
  for (const auto count : counts)
  {
    std::ostringstream oss;
    oss << "best_candidate_" << count << ".bmp";
    save_points(points, count, oss.str());
  }
}
