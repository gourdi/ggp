#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_scale2d.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, nearest_neighbor_upsample)
{
  const ggo::array2d_32s input({
    { 1, 2, 3 },
    { 4, 5, 6 } });
  ggo::array2d_32s output(6, 4);

  ggo::scale_2d<ggo::scaling_algo::nearest_neighbor, ggo::scaling_algo::nearest_neighbor>(
    input.data(), input.width(), input.height(), int(sizeof(int32_t) * input.width()),
    output.data(), output.width(), output.height(), int(sizeof(int32_t) * output.width()));

  const ggo::array2d_32s expected({
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
    const ggo::array2d_32f input({
      { 1.f, 2.f, 3.f },
      { 4.f, 5.f, 6.f } });
    ggo::array2d_32f output(5, 3);

    ggo::scale_2d<ggo::scaling_algo::linear_integration, ggo::scaling_algo::linear_integration>(
      input.data(), input.width(), input.height(), int(sizeof(int32_t) * input.width()),
      output.data(), output.width(), output.height(), int(sizeof(int32_t) * output.width()));

    const ggo::array2d_32f expected({
      { 1.7f, 2.1f, 2.5f, 2.9f, 3.3f },
      { 2.7f, 3.1f, 3.5f, 3.9f, 4.3f },
      { 3.7f, 4.1f, 4.5f, 4.9f, 5.3f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }

  {
    const ggo::array2d_32f input({
      { 1.f, 1.f, 1.f },
      { 1.f, 1.f, 2.f } });
    ggo::array2d_32f output(6, 4);

    ggo::scale_2d<ggo::scaling_algo::linear_integration, ggo::scaling_algo::linear_integration>(
      input.data(), input.width(), input.height(), int(sizeof(int32_t) * input.width()),
      output.data(), output.width(), output.height(), int(sizeof(int32_t) * output.width()));

    const ggo::array2d_32f expected({
      { 1.f, 1.f, 1.f, 1.02083325f, 1.0625f, 1.10416651f },
      { 1.f, 1.f, 1.f, 1.0625f,     1.1875f, 1.3125f     },
      { 1.f, 1.f, 1.f, 1.10416698f, 1.3125f, 1.52083290f },
      { 1.f, 1.f, 1.f, 1.14583349f, 1.4375f, 1.72916603f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }
}

