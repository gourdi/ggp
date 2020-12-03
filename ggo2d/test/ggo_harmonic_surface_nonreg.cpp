#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec.h>
#include <kernel/math/scalar_fields_2d/ggo_harmonics_field_2d.h>
#include <2d/processing/ggo_blit.h>
#include <2d/io/ggo_bmp.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(harmonic_surface, test1)
{
  const int size = 200;
  
  ggo::harmonics_field_2d<float> harmonics;
  harmonics.push_harmonic({ 1.f, 1.f }, size / 2, 1, 0);

  ggo::image_t<ggo::pixel_type::y_32f> image_y32f({ size, size });
  for (int y = 0; y < image_y32f.height(); ++y)
  {
    for (int x = 0; x < image_y32f.width(); ++x)
    {
      float value = ggo::map(harmonics.sample(static_cast<float>(x), static_cast<float>(y)), -1.f, 1.f, 0.f, 1.f);
      image_y32f.write_pixel(x, y, value);
    }
  }

  ggo::image_rgb_8u image_rgb8u({ size, size });
  ggo::blit(image_rgb8u, image_y32f, { 0, 0 });
  
  ggo::save_bmp("test_harmonic_surface1.bmp", image_rgb8u);
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

    harmonics.push_harmonic(length * ggo::vec2_f::from_angle(angle), wavelength, amplitude, 0);
  }

  ggo::image_t<ggo::pixel_type::y_32f> image_y32f({ size, size });
  for (int y = 0; y < image_y32f.height(); ++y)
  {
    for (int x = 0; x < image_y32f.width(); ++x)
    {
      float value = ggo::map<float>(harmonics.sample(static_cast<float>(x), static_cast<float>(y)), -4.f, 4.f, 0.f, 1.f);
      image_y32f.write_pixel(x, y, value);
    }
  }

  ggo::image_rgb_8u image_rgb8u({ size, size });
  ggo::blit(image_rgb8u, image_y32f, { 0, 0 });
  
  ggo::save_bmp("test_harmonic_surface2.bmp", image_rgb8u);
}

