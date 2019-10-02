#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/processing/ggo_local_processing.h>

////////////////////////////////////////////////////////////////////
template <ggo::lines_order memory_lines_order>
bool test_horizontal_lines()
{
  const std::array<float, 10> input_pixels{ {
  1.f, 5.f, 3.f, 2.f, 4.f,
  2.f, 1.f, 4.f, 3.f, 2.f } };
  std::array<float, 10> output_pixels;
  output_pixels.fill(0.f);

  ggo::const_image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::up> input_image(input_pixels.data(), { 5, 2 });
  ggo::image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::up> output_image(output_pixels.data(), { 5, 2 });

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };

  ggo::apply_horizontal_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing);

  const std::array<float, 10> expected_pixels{ {
     4.f, -2.f, -1.f,  2.f, -4.f,
    -1.f,  3.f, -1.f, -1.f, -2.f } };
  return ggo::compare(output_pixels, expected_pixels, 0.00001f);
}

GGO_TEST(image_local_processing, horizontal_lines)
{
  GGO_CHECK(test_horizontal_lines<ggo::lines_order::up>());
  GGO_CHECK(test_horizontal_lines<ggo::lines_order::down>());
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_lines_up)
{
  const std::array<float, 12> input_pixels{ {
    1.f, 5.f, 3.f,
    2.f, 1.f, 4.f,
    2.f, 4.f, 1.f,
    5.f, 1.f, 2.f} };
  std::array<float, 12> output_pixels;
  output_pixels.fill(0.f);

  ggo::const_image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::up> input_image(input_pixels.data(), { 3, 4 });
  ggo::image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::up> output_image(output_pixels.data(), { 3, 4 });

  auto processing_up = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };
  auto symmetric_down = [](const auto & neighborhood)
  {
    return neighborhood(-1) - neighborhood(0);
  };

  ggo::apply_vertical_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing_up, symmetric_down);

  const std::array<float, 12> expected_pixels{ {
      1.f, -4.f,  1.f,
      0.f,  3.f, -3.f,
      3.f, -3.f,  1.f,
     -5.f, -1.f, -2.f } };
  GGO_CHECK(ggo::compare(output_pixels, expected_pixels, 0.00001f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_lines_down)
{
  const std::array<float, 12> input_pixels{ {
    5.f, 1.f, 2.f,
    2.f, 4.f, 1.f,
    2.f, 1.f, 4.f,
    1.f, 5.f, 3.f,} };
  std::array<float, 12> output_pixels;
  output_pixels.fill(0.f);

  ggo::const_image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::down> input_image(input_pixels.data(), { 3, 4 });
  ggo::image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::down> output_image(output_pixels.data(), { 3, 4 });

  auto processing_up = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };
  auto processing_down = [](const auto & neighborhood)
  {
    return neighborhood(-1) - neighborhood(0);
  };

  ggo::apply_vertical_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing_up, processing_down);

  const std::array<float, 12> expected_pixels{ {
     -5.f, -1.f, -2.f,
      3.f, -3.f,  1.f,
      0.f,  3.f, -3.f,
      1.f, -4.f,  1.f } };
  GGO_CHECK(ggo::compare(output_pixels, expected_pixels, 0.00001f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_symmetric)
{
  const std::array<float, 12> input_pixels{ {
    5.f, 1.f, 2.f,
    2.f, 4.f, 1.f,
    2.f, 1.f, 4.f,
    1.f, 5.f, 3.f,} };
  std::array<float, 12> output_pixels;
  output_pixels.fill(0.f);

  ggo::const_image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::down> input_image(input_pixels.data(), { 3, 4 });
  ggo::image_view_t<ggo::pixel_type::y_32f, ggo::lines_order::down> output_image(output_pixels.data(), { 3, 4 });

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(-1) + neighborhood(0) + neighborhood(1);
  };

  ggo::apply_vertical_symmetric_processing<ggo::border_mode::mirror>(input_image, output_image, 1, processing);

  const std::array<float, 12> expected_pixels{ {
     12.f,  6.f,  5.f,
      9.f,  6.f,  7.f,
      5.f, 10.f,  8.f,
      4.f, 11.f, 10.f } };
  GGO_CHECK(ggo::compare(output_pixels, expected_pixels, 0.00001f));
}

