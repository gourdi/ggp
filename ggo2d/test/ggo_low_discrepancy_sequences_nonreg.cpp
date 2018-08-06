#include <sstream>
#include <kernel/memory/ggo_array.h>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_halton.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.h>
#include <2d/paint/ggo_paint.h>
#include <2d/io/ggo_bmp.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  void save_points(const std::vector<ggo::pos2f> & points, int points_count, const std::string & filename)
  {
    const int size = 400;
    
    ggo::array_8u image(3 * size * size, 0);

    for (int i = 0; i < points_count; ++i)
    {
      ggo::pos2f center = static_cast<float>(size) * points[i];
      ggo::paint<ggo::rgb_8u_yd, ggo::sampling_2x2>(image.data(), size, size, 3 * size, ggo::disc_float(center, 2.f), ggo::white_8u());
    }
    
    ggo::save_bmp(filename, image.data(), ggo::rgb_8u_yd, size, size, 3 * size);
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

