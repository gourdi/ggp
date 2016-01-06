#include <ggo_nonreg.h>
#include <ggo_pixel_sampler_abc.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_sampling, sampler2X2)
{
  ggo::pixel_sampler_2X2 sampler;
  
  // Counting.
  float sum = 0;
  int counter = 0;
  sampler.sample_pixel(1, 1, [&](float x, float y)
  {
    sum += 1.f / sampler.get_samples_count();
    counter++;
  });
  GGO_CHECK_FABS(sum, 1.f);
  GGO_CHECK(counter == 4);
  
  // Shape sampling.
  ggo::disc_float disc(0.5, 0.5, 0.5);
  float color = 0;
  sampler.sample_shape(1, 1, disc, [&](float x, float y)
  {
    color += 1;
  });
  color /= sampler.get_samples_count();
  GGO_CHECK_FABS(color, 0.25f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_sampling, sampler4X4)
{
  ggo::pixel_sampler_4X4 sampler;

  // Counting.
  float sum = 0;
  int counter = 0;
  sampler.sample_pixel(1, 1, [&](float x, float y)
  {
    sum += 1.f / sampler.get_samples_count();
    counter++;
  });
  GGO_CHECK_FABS(sum, 1.f);
  GGO_CHECK(counter == 16);

  // Shape sampling.
  ggo::polygon2d_float polygon;
  polygon.add_point(0, 0);
  polygon.add_point(0.75, 0);
  polygon.add_point(0.75, 1);
  polygon.add_point(0, 1);
  float color = 0;
  sampler.sample_shape(1, 1, polygon, [&](float x, float y)
  {
    color += 1;
  });  
  color /= sampler.get_samples_count();
  GGO_CHECK_FABS(color, 0.125f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_sampling, sampler8X8)
{
  ggo::pixel_sampler_8X8 sampler;

  // Counting.
  float sum = 0;
  int counter = 0;
  sampler.sample_pixel(1, 1, [&](float x, float y)
  {
    sum += 1.f / sampler.get_samples_count();
    counter++;
  });
  GGO_CHECK_FABS(sum, 1.f);
  GGO_CHECK(counter == 64);

  // Shape sampling.
  float left   = 0.25f;
  float right  = 1;
  float bottom = 1 / 8.f;
  float top  = 1;

  ggo::polygon2d_float polygon;
  polygon.add_point(left, bottom);
  polygon.add_point(right, bottom);
  polygon.add_point(right, top);
  polygon.add_point(left, top);
  
  counter = 0;
  sampler.sample_shape(0, 0, polygon, [&](float x, float y)
  {
    ++counter;
  });
  GGO_CHECK(counter == 6);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_sampling, sampler16X16)
{
  ggo::pixel_sampler_16X16 sampler;

  // Counting.
  float sum = 0;
  int counter = 0;
  sampler.sample_pixel(1, 1, [&](float x, float y)
  {
    sum += 1.f / sampler.get_samples_count();
    counter++;
  });
  GGO_CHECK_FABS(sum, 1.f);
  GGO_CHECK(counter == 256);
}
