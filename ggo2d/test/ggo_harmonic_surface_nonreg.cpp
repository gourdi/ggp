#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec.h>
#include <kernel/math/scalar_fields_2d/ggo_harmonics_field_2d.h>
#include <2d/io/ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(harmonic_surface, test1)
{
  const int size = 200;
  
  ggo::harmonics_field_2d<float> harmonics;
  harmonics.push_harmonic({1.f, 1.f}, size / 2, 1, 0);

  std::vector<float> image(size * size);
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      float value = ggo::map(harmonics.sample(static_cast<float>(x), static_cast<float>(y)), -1.f, 1.f, 0.f, 1.f);
      ggo::write_pixel<ggo::y_32f_yu>(image.data(), x, y, size, sizeof(float) * size, value);
    }
  }
  
  ggo::save_bmp("test_harmonic_surface1.bmp", image.data(), ggo::y_32f_yu, size, size, sizeof(float) * size);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(harmonic_surface, test2)
{
  const int size = 200;
  
  ggo::harmonics_field_2d<float> harmonics;
  for (int i = 0; i < 4; ++i)
  {
    float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    float length = 1;
    float wavelength = static_cast<float>(size >> (i + 1));
    float amplitude = 1.f / static_cast<float>(i + 1);
    float phase = 0;

    harmonics.push_harmonic(length * ggo::vec2f::from_angle(angle), wavelength, amplitude, 0);
  }

  std::vector<float> image(size * size);
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      float value = ggo::map<float>(harmonics.sample(static_cast<float>(x), static_cast<float>(y)), -4.f, 4.f, 0.f, 1.f);
      ggo::write_pixel<ggo::y_32f_yu>(image.data(), x, y, size, sizeof(float) * size, value);
    }
  }
  
  ggo::save_bmp("test_harmonic_surface2.bmp", image.data(), ggo::y_32f_yu, size, size, sizeof(float) * size);
}

