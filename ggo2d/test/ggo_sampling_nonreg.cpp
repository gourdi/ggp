#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <ggo_sampling_paint.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sampling, range_and_count)
{
  {
    int count = 0;
    ggo::sampler<ggo::sampling_1>::sample_pixel<float>(1, 2, [&](float x_f, float y_f)
    {
      GGO_CHECK_FLOAT_EQ(x_f, 1.5f);
      GGO_CHECK_FLOAT_EQ(y_f, 2.5f);
      ++count;
    });
    GGO_CHECK_EQ(count, ggo::sampler<ggo::sampling_1>::samples_count);
  }

  {
    int count = 0;
    ggo::sampler<ggo::sampling_2x2>::sample_pixel<float>(1, 2, [&](float x_f, float y_f)
    {
      GGO_CHECK(x_f > 1.f && x_f < 2.f);
      GGO_CHECK(y_f > 2.f && y_f < 3.f);
      ++count;
    });
    GGO_CHECK_EQ(count, ggo::sampler<ggo::sampling_2x2>::samples_count);
  }

  {
    int count = 0;
    ggo::sampler<ggo::sampling_4x4>::sample_pixel<float>(1, 2, [&](float x_f, float y_f)
    {
      GGO_CHECK(x_f > 1.f && x_f < 2.f);
      GGO_CHECK(y_f > 2.f && y_f < 3.f);
      ++count;
    });
    GGO_CHECK_EQ(count, ggo::sampler<ggo::sampling_4x4>::samples_count);
  }

  {
    int count = 0;
    ggo::sampler<ggo::sampling_8x8>::sample_pixel<float>(1, 2, [&](float x_f, float y_f)
    {
      GGO_CHECK(x_f > 1.f && x_f < 2.f);
      GGO_CHECK(y_f > 2.f && y_f < 3.f);
      ++count;
    });
    GGO_CHECK_EQ(count, ggo::sampler<ggo::sampling_8x8>::samples_count);
  }

  {
    int count = 0;
    ggo::sampler<ggo::sampling_16x16>::sample_pixel<float>(1, 2, [&](float x_f, float y_f)
    {
      GGO_CHECK(x_f > 1.f && x_f < 2.f);
      GGO_CHECK(y_f > 2.f && y_f < 3.f);
      ++count;
    });
    GGO_CHECK_EQ(count, ggo::sampler<ggo::sampling_16x16>::samples_count);
  }
}
