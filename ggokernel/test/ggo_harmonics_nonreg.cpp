#include <ggo_nonreg.h>
#include <ggo_vec.h>
#include <ggo_kernel.h>
#include <ggo_harmonic_surface.h>

GGO_TEST(harmonic_surface, harmonic_surface)
{
  ggo::harmonic_surface<float> harmonic_surface;
  
  harmonic_surface.push_harmonic({1.f, 1.f}, 100, 1, 0);
  
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(0, 0), 1);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(100 / std::sqrt(2.f), 100 / std::sqrt(2.f)), 1);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(100 * std::sqrt(2.f), 0), 1);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(0, 100 * std::sqrt(2.f)), 1);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(0, 75 * std::sqrt(2.f)), 0);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(0, 50 * std::sqrt(2.f)), -1);
  GGO_CHECK_FLOAT_EQ(harmonic_surface.evaluate(0, 25 * std::sqrt(2.f)), 0);
}