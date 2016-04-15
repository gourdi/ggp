#include <ggo_nonreg.h>
#include <ggo_set2.h>
#include <ggo_harmonic_surface.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(harmonic_surface, test1)
{
  const int SIZE = 200;
  
  ggo::harmonic_surface<float> harmonic_surface;
  harmonic_surface.push_harmonic({1, 1}, SIZE / 2, 1, 0);

  ggo::rgb_image_buffer_uint8 image(SIZE, SIZE);
  for (int y = 0; y < SIZE; ++y)
  {
    for (int x = 0; x < SIZE; ++x)
    {
      float value = ggo::map(harmonic_surface.evaluate(static_cast<float>(x), static_cast<float>(y)), -1.f, 1.f, 0.f, 1.f);
      image.write(x, y, ggo::color(value));
    }
  }
  
  ggo::save_bmp("test_harmonic_surface1.bmp", image.get_buffer(), SIZE, SIZE);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(harmonic_surface, test2)
{
  const int SIZE = 200;
  
  ggo::harmonic_surface<float> harmonic_surface;
  for (int i = 0; i < 4; ++i)
  {
    float angle = ggo::rand_float(0, 2 * ggo::PI<float>());
    float length = 1;
    float wavelength = static_cast<float>(SIZE >> (i + 1));
    float amplitude = 1.f / static_cast<float>(i + 1);
    float phase = 0;

    harmonic_surface.push_harmonic(ggo::vector2d_float::from_polar(angle, length), wavelength, amplitude, 0);
  }

  ggo::rgb_image_buffer_uint8 image(SIZE, SIZE);
  for (int y = 0; y < SIZE; ++y)
  {
    for (int x = 0; x < SIZE; ++x)
    {
      float value = ggo::map<float>(harmonic_surface.evaluate(static_cast<float>(x), static_cast<float>(y)), -4.f, 4.f, 0.f, 1.f);
      image.write(x, y, ggo::color(value));
    }
  }
  
  ggo::save_bmp("test_harmonic_surface2.bmp", image.get_buffer(), SIZE, SIZE);
}
