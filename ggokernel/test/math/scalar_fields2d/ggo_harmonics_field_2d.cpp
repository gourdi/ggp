#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/scalar_fields_2d/ggo_harmonics_field_2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(harmonics_field_2d, harmonic_surface)
{
  ggo::harmonics_field_2d<float> harmonics;
  
  harmonics.push_harmonic({1.f, 1.f}, 100, 1, 0);
  
  GGO_CHECK_FLOAT_EQ(harmonics.sample(0, 0), 1);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(100 / std::sqrt(2.f), 100 / std::sqrt(2.f)), 1);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(100 * std::sqrt(2.f), 0), 1);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(0, 100 * std::sqrt(2.f)), 1);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(0, 75 * std::sqrt(2.f)), 0);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(0, 50 * std::sqrt(2.f)), -1);
  GGO_CHECK_FLOAT_EQ(harmonics.sample(0, 25 * std::sqrt(2.f)), 0);
}