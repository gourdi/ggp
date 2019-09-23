#include <array>
#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_processing/ggo_local_processing.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(local_processing, mean_border_zero_1d)
{
  auto processing = [](const auto & neighborhood)
  {
    return (neighborhood(-1) + neighborhood(0) + neighborhood(1)) / 3;
  };

  const std::array<float, 5> input{ { 1.f, 1.f, 2.f, 3.f, 4.f } };
  std::array<float, 5> output;

  ggo::processing_1d<ggo::border_mode::zero>(input.data(), input.data() + input.size(), output.data(), 3, processing);

  GGO_CHECK_FLOAT_EQ(output[0], 2.f / 3.f);
  GGO_CHECK_FLOAT_EQ(output[1], 4.f / 3.f);
  GGO_CHECK_FLOAT_EQ(output[2], 2.f);
  GGO_CHECK_FLOAT_EQ(output[3], 3.f);
  GGO_CHECK_FLOAT_EQ(output[4], 7.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(local_processing, sum_border_mirror_1d)
{
  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(-2) + neighborhood(-1) + neighborhood(0) + neighborhood(1) + neighborhood(2);
  };

  const std::array<int, 5> input{ { 1, 2, 3, 4, 5 } };
  std::array<int, 5> output;

  ggo::processing_1d<ggo::border_mode::mirror>(input.data(), input.data() + input.size(), output.data(), 5, processing);

  GGO_CHECK_EQ(output[0], 9);
  GGO_CHECK_EQ(output[1], 11);
  GGO_CHECK_EQ(output[2], 15);
  GGO_CHECK_EQ(output[3], 19);
  GGO_CHECK_EQ(output[4], 21);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(local_processing, array_2d_horizontal)
{
  auto processing = [](const auto & neighborhood)
  {
    return (neighborhood(-1) + neighborhood(0) + neighborhood(1)) / 3;
  };

  const ggo::array2d_32f input({
    {2.f, 3.f, 4.f},
    {4.f, 3.f, 2.f} });
  ggo::array2d_32f output(input.size());

  ggo::apply_horizontal_processing<ggo::border_mode::zero>(input, output, 3, processing);

  GGO_CHECK_EQ(output(0, 0), 5.f / 3.f);
  GGO_CHECK_EQ(output(1, 0), 3.f);
  GGO_CHECK_EQ(output(2, 0), 7.f / 3.f);
  GGO_CHECK_EQ(output(0, 1), 7.f / 3.f);
  GGO_CHECK_EQ(output(1, 1), 3.f);
  GGO_CHECK_EQ(output(2, 1), 5.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(local_processing, array_2d_vertical)
{
  auto processing = [](const auto & neighborhood)
  {
    return (neighborhood(-1) + neighborhood(0) + neighborhood(1)) / 3;
  };

  const ggo::array2d_32f input({
    {2.f, 4.f},
    {3.f, 3.f},
    {4.f, 2.f} });
  ggo::array2d_32f output(input.size());

  ggo::apply_vertical_processing<ggo::border_mode::zero>(input, output, 3, processing);

  GGO_CHECK_EQ(output(0, 0), 5.f / 3.f);
  GGO_CHECK_EQ(output(0, 1), 3.f);
  GGO_CHECK_EQ(output(0, 2), 7.f / 3.f);
  GGO_CHECK_EQ(output(1, 0), 7.f / 3.f);
  GGO_CHECK_EQ(output(1, 1), 3.f);
  GGO_CHECK_EQ(output(1, 2), 5.f / 3.f);
}




