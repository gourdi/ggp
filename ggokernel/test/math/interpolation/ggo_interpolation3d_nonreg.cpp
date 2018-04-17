#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation3d.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation3d, test)
{
  std::array<int, 12> data{ {
      1, 2, 2,
      3, 0, 1,
      1, 0, 2,
      4, 5, 2 } };
                            
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation3d(data.data(), 3, 2, 2, 1.f, 1.f, 0.f), 0.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation3d(data.data(), 3, 2, 2, 2.f, 0.5f, 1.f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation3d(data.data(), 3, 2, 2, 2.f, 1.f, 0.25f), 1.25f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation3d(data.data(), 3, 2, 2, 0.5f, 0.5f, 0.5f), 2.f);
}
