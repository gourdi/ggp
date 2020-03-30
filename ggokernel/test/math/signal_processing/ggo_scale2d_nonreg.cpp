#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, nearest_neighbor_upsample)
{
  const ggo::array2_i input({
    { 1, 2, 3 },
    { 4, 5, 6 } });
  ggo::array2_i output(4, 6);

  //ggo::scale_2d<ggo::scaling_algo::nearest_neighbor>(
  //  input.data(), input.width(), input.height(),
  //  output.data(), output.width(), output.height());

  const ggo::array2_i expected({
    { 1, 1, 2, 2, 3, 3 },
    { 1, 1, 2, 2, 3, 3 },
    { 4, 4, 5, 5, 6, 6 },
    { 4, 4, 5, 5, 6, 6 } });

  GGO_CHECK_EQ(output, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, linear_upsample)
{
  {
    const ggo::array2_f input({
      { 1.f, 2.f, 3.f },
      { 4.f, 5.f, 6.f } });
    ggo::array2_f output(3, 5);

    //ggo::scale_2d<ggo::scaling_algo::linear_integration>(
    //  input.data(), input.width(), input.height(),
    //  output.data(), output.width(), output.height());

    const ggo::array2_f expected({
      { 1.7f, 2.1f, 2.5f, 2.9f, 3.3f },
      { 2.7f, 3.1f, 3.5f, 3.9f, 4.3f },
      { 3.7f, 4.1f, 4.5f, 4.9f, 5.3f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }

  {
    const ggo::array2_f input({
      { 1.f, 1.f, 1.f },
      { 1.f, 1.f, 2.f } });
    ggo::array2_f output(4, 6);

    //ggo::scale_2d<ggo::scaling_algo::linear_integration>(
    //  input.data(), input.width(), input.height(),
    //  output.data(), output.width(), output.height());

    const ggo::array2_f expected({
      { 1.f, 1.f, 1.f, 1.02083325f, 1.0625f, 1.10416651f },
      { 1.f, 1.f, 1.f, 1.0625f,     1.1875f, 1.3125f     },
      { 1.f, 1.f, 1.f, 1.10416698f, 1.3125f, 1.52083290f },
      { 1.f, 1.f, 1.f, 1.14583349f, 1.4375f, 1.72916603f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, resample_bilinear)
{
  //constexpr int width_in = 2;
  //constexpr int height_in = 2;
  //const float in[width_in * height_in] = {
  //  0.0f, 1.0f,
  //  0.0f, 2.0f };

  //constexpr int width_out = 4;
  //constexpr int height_out = 4;
  //float out[width_out * height_out] = { 0 };

  //ggo::scale_2d<ggo::scaling_algo::linear_resampling>(in, width_in, height_in, out, width_out, height_out);

  //GGO_CHECK_FLOAT_EQ(out[0], 0.0f);
  //GGO_CHECK_FLOAT_EQ(out[1], 1.0f / 3.0f);
  //GGO_CHECK_FLOAT_EQ(out[2], 2.0f / 3.0f);
  //GGO_CHECK_FLOAT_EQ(out[3], 1.0f);

  //GGO_CHECK_FLOAT_EQ(out[4], 0.0f);
  //GGO_CHECK_FLOAT_EQ(out[5], 4.0f / 9.0f);
  //GGO_CHECK_FLOAT_EQ(out[6], 8.0f / 9.0f);
  //GGO_CHECK_FLOAT_EQ(out[7], 12.0f / 9.0f);

  //GGO_CHECK_FLOAT_EQ(out[8], 0.0f);
  //GGO_CHECK_FLOAT_EQ(out[9], 5.0f / 9.0f);
  //GGO_CHECK_FLOAT_EQ(out[10], 10.0f / 9.0f);
  //GGO_CHECK_FLOAT_EQ(out[11], 15.0f / 9.0f);

  //GGO_CHECK_FLOAT_EQ(out[12], 0.0f);
  //GGO_CHECK_FLOAT_EQ(out[13], 2.0f / 3.0f);
  //GGO_CHECK_FLOAT_EQ(out[14], 4.0f / 3.0f);
  //GGO_CHECK_FLOAT_EQ(out[15], 2.0f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, resample_bicubic)
{
  //constexpr int width_in = 3;
  //constexpr int height_in = 2;
  //const float in[width_in * height_in] = {
  //  1.0f, 0.0f, 0.0f,
  //  1.0f, 0.0f, 1.0f };

  //constexpr int width_out = 4;
  //constexpr int height_out = 3;
  //float out[width_out * height_out] = { 0 };

  //ggo::scale_2d<ggo::scaling_algo::cubic_resampling>(in, width_in, height_in, out, width_out, height_out);

  //GGO_CHECK_FLOAT_EQ(out[0], 1.00000000f);
  //GGO_CHECK_FLOAT_EQ(out[1], 0.296296239f);
  //GGO_CHECK_FLOAT_EQ(out[2], -0.0740740746f);
  //GGO_CHECK_FLOAT_EQ(out[3], 0.000000000f);

  //GGO_CHECK_FLOAT_EQ(out[4], 1.00000000f);
  //GGO_CHECK_FLOAT_EQ(out[5], 0.259259224f);
  //GGO_CHECK_FLOAT_EQ(out[6], 0.0740741044f);
  //GGO_CHECK_FLOAT_EQ(out[7], 0.500000000f);

  //GGO_CHECK_FLOAT_EQ(out[8], 1.00000000f);
  //GGO_CHECK_FLOAT_EQ(out[9], 0.222222209f);
  //GGO_CHECK_FLOAT_EQ(out[10], 0.222222209f);
  //GGO_CHECK_FLOAT_EQ(out[11], 1.000000008f);
}



