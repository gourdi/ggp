#include <ggo_nonreg.h>
#include <ggo_scale1d.h>
#include <ggo_buffer.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, nearest_neighbor_upsample)
{
  const ggo::buffer_t<float> input{ 1.f, 2.f };
  ggo::buffer_t<float> output(20);

  ggo::scale_1d<ggo::scaling_algo::nearest_neighbor>(input.data(), input.get_size(), output.data(), output.get_size());

  const ggo::buffer_t<float> expected{
    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
    2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f };

  GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin()));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, nearest_neighbor_downsample)
{
  const ggo::buffer_t<float> input{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };
  ggo::buffer_t<float> output(2);

  ggo::scale_1d<ggo::scaling_algo::nearest_neighbor>(input.data(), input.get_size(), output.data(), output.get_size());

  const ggo::buffer_t<float> expected{ 3.f, 7.f };

  GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin()));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, linear_upsample)
{
  const ggo::buffer_t<float> input{ 1.f, 2.f };
  ggo::buffer_t<float> output(20);

  ggo::scale_1d<ggo::scaling_algo::linear_integration>(input.data(), input.get_size(), output.data(), output.get_size());

  const ggo::buffer_t<float> expected{
    1.025f, 1.075f, 1.125f, 1.175f, 1.225f, 1.275f, 1.325f, 1.375f, 1.425f, 1.475f,
    1.525f, 1.575f, 1.625f, 1.675f, 1.725f, 1.775f, 1.825f, 1.875f, 1.925f, 1.975f };

  for (int i = 0; i < output.get_size(); ++i)
  {
    GGO_CHECK_FLOAT_NEAR(output[i], expected[i], 0.001f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, linear_downsample)
{
  const ggo::buffer_t<float> input{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f };
  ggo::buffer_t<float> output(2);

  ggo::scale_1d<ggo::scaling_algo::linear_integration>(input.data(), input.get_size(), output.data(), output.get_size());

  const ggo::buffer_t<float> expected{ 2.5f, 5.5f };

  for (int i = 0; i < output.get_size(); ++i)
  {
    GGO_CHECK_FLOAT_NEAR(output[i], expected[i], 0.001f);
  }
}

