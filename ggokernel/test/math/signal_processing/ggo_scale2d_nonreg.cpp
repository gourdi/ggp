#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_processing/ggo_scale2d.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, nearest_neighbor)
{
  const ggo::array2_i input({
    { 1, 2, 3 },
    { 4, 5, 6 } });
  ggo::array2_i output(4, 6);

  ggo::scale_2d_nearest_neighbor(
    input.data(), input.width(), input.height(),
    output.data(), output.width(), output.height());

  const ggo::array2_i expected({
    { 1, 1, 2, 2, 3, 3 },
    { 1, 1, 2, 2, 3, 3 },
    { 4, 4, 5, 5, 6, 6 },
    { 4, 4, 5, 5, 6, 6 } });

  GGO_CHECK_EQ(output, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, linear_interpolation)
{
  {
    const ggo::array2_f input({
      { 1.f, 2.f, 3.f },
      { 4.f, 5.f, 6.f } });
    ggo::array2_f output(3, 5);

    ggo::scale_2d_linear_interpolation(
      input.data(), input.width(), input.height(),
      output.data(), output.width(), output.height());

    const ggo::array2_f expected({
      { 1.0f, 1.5f, 2.0f, 2.5f, 3.0f },
      { 2.5f, 3.0f, 3.5f, 4.0f, 4.5f },
      { 4.0f, 4.5f, 5.0f, 5.5f, 6.0f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }

  {
    const ggo::array2_f input({
      { 1.f, 1.f, 1.f },
      { 1.f, 1.f, 2.f } });
    ggo::array2_f output(4, 6);

    ggo::scale_2d_linear_interpolation(
      input.data(), input.width(), input.height(),
      output.data(), output.width(), output.height());

    const ggo::array2_f expected({
      { 1.f, 1.f, 1.f, 1.f,     1.f,  1.f },
      { 1.f, 1.f, 1.f, 1.0666f, 1.2f, 1.3333f },
      { 1.f, 1.f, 1.f, 1.1333f, 1.4f, 1.6666f },
      { 1.f, 1.f, 1.f, 1.2f,    1.6f, 2.0f } });

    GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scale2d, cubic_interpolation)
{
  const ggo::array2_f input({
    { 1.f, 1.f, 1.f },
    { 1.f, 1.f, 2.f } });
  ggo::array2_f output(4, 6);

  ggo::scale_2d_cubic_interpolation(
    input.data(), input.width(), input.height(),
    output.data(), output.width(), output.height());

  const ggo::array2_f expected({
    { 1.f, 1.f,          1.f,          1.f,         1.f,         1.f },
    { 1.f, 0.985777795f, 0.981037021f, 1.04503703f, 1.18488896f, 1.29629636f },
    { 1.f, 0.966222227f, 0.954962969f, 1.10696304f, 1.43911123f, 1.70370376f },
    { 1.f, 0.952000022f, 0.935999990f, 1.15200007f, 1.62400007f, 2.0f } });

  GGO_CHECK(std::equal(output.begin(), output.end(), expected.begin(), expected.end(), [](float v1, float v2) { return std::abs(v1 - v2) < 0.001f; }));
}
