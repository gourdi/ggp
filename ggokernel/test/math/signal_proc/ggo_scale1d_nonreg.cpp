#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_scale1d.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, nearest_neighbor_upsample)
{
  const ggo::array_32f input({ 1.f, 2.f });
  ggo::array_32f output(20);

  ggo::scale_1d<ggo::scaling_algo::nearest_neighbor>(input.data(), input.size(), output.data(), output.size());

  const ggo::array_32f expected({
    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
    2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });

  GGO_CHECK_EQ(output, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, nearest_neighbor_downsample)
{
  const ggo::array_32f input({ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f });
  ggo::array_32f output(2);

  ggo::scale_1d<ggo::scaling_algo::nearest_neighbor>(input.data(), input.size(), output.data(), output.size());

  const ggo::array_32f expected({ 3.f, 7.f });

  GGO_CHECK_EQ(output, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, linear_upsample)
{
  {
    const ggo::array_32f input({ 1.f, 2.f });
    ggo::array_32f output(20);

    ggo::scale_1d<ggo::scaling_algo::linear_integration>(input.data(), input.size(), output.data(), output.size());

    const ggo::array_32f expected({
      1.025f, 1.075f, 1.125f, 1.175f, 1.225f, 1.275f, 1.325f, 1.375f, 1.425f, 1.475f,
      1.525f, 1.575f, 1.625f, 1.675f, 1.725f, 1.775f, 1.825f, 1.875f, 1.925f, 1.975f });

    for (int i = 0; i < output.size(); ++i)
    {
      GGO_CHECK_FLOAT_NEAR(output[i], expected[i], 0.001f);
    }
  }

  {
    const ggo::array_32f input({1.f, 1.f, 1.f, 2.f, 2.f, 2.f });
    ggo::array_32f output(10);

    ggo::scale_1d<ggo::scaling_algo::linear_integration>(input.data(), input.size(), output.data(), output.size());

    const ggo::array_32f expected({ 1.f, 1.f, 1.f, 1.f, 1.25f, 1.75f, 2.f, 2.f, 2.f, 2.f });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin()));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, linear_downsample)
{
  const ggo::array_32f input({ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f });
  ggo::array_32f output(2);

  ggo::scale_1d<ggo::scaling_algo::linear_integration>(input.data(), input.size(), output.data(), output.size());

  const ggo::array_32f expected({ 2.5f, 5.5f });

  for (int i = 0; i < output.size(); ++i)
  {
    GGO_CHECK_FLOAT_NEAR(output[i], expected[i], 0.001f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale1d, cubic_upsample)
{
  const ggo::array_32f input({ 1.f, 1.f, 1.f, 2.f, 2.f, 2.f });
  ggo::array_32f output(20);

  ggo::scale_1d<ggo::scaling_algo::cubic_integration>(input.data(), input.size(), output.data(), output.size());

  const ggo::array_32f expected({
    1.f, 1.f, 1.f, 1.f,
    0.9915363f, 0.9563802f, 0.9290364f, 0.9563798f,
    1.0898437f, 1.3476562f, 1.6523437f, 1.9101562f,
    2.0436201f, 2.0709633f, 2.0436196f, 2.0084648f,
    2.f, 2.f, 2.f, 2.f });

  for (int i = 0; i < output.size(); ++i)
  {
    GGO_CHECK_FLOAT_NEAR(output[i], expected[i], 0.001f);
  }
}

