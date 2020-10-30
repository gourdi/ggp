#include "ggo_2d_nonreg.h"
#include <kernel/ggo_compare.h>
#include <2d/processing/ggo_local_processing.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, horizontal_rows_up)
{
  const auto input_image = make_image_t<ggo::pixel_type::y_32f>({ 5, 2 }, {
    1.f, 5.f, 3.f, 2.f, 4.f,
    2.f, 1.f, 4.f, 3.f, 2.f });
  auto output_image = make_image_t<ggo::pixel_type::y_32f>({ 5, 2 }, 0.f);

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };

  ggo::apply_horizontal_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing);

  GGO_CHECK_PIXELS(output_image, {
     4.f, -2.f, -1.f,  2.f, -4.f ,
    -1.f,  3.f, -1.f, -1.f, -2.f });
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, horizontal_rows_down)
{
  const auto input_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 5, 2 }, {
    1.f, 5.f, 3.f, 2.f, 4.f,
    2.f, 1.f, 4.f, 3.f, 2.f });
  auto output_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 5, 2 }, 0.f);

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };

  ggo::apply_horizontal_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing);

  GGO_CHECK_PIXELS(output_image, {
     4.f, -2.f, -1.f,  2.f, -4.f ,
    -1.f,  3.f, -1.f, -1.f, -2.f });
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_lines_up)
{
  const auto input_image = make_image_t<ggo::pixel_type::y_32f>({ 3, 4 }, {
    1.f, 5.f, 3.f,
    2.f, 1.f, 4.f,
    2.f, 4.f, 1.f,
    5.f, 1.f, 2.f });
  auto output_image = make_image_t<ggo::pixel_type::y_32f>({3, 4}, 0.f);

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };

  ggo::apply_vertical_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing);

  GGO_CHECK_PIXELS(output_image, {
     1.f, -4.f,  1.f,
     0.f,  3.f, -3.f,
     3.f, -3.f,  1.f,
    -5.f, -1.f, -2.f });
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_lines_down)
{
  const auto input_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 3, 4 }, {
     5.f, 1.f, 2.f,
     2.f, 4.f, 1.f,
     2.f, 1.f, 4.f,
     1.f, 5.f, 3.f });
  auto output_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 3, 4 }, 0.f);

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(1) - neighborhood(0);
  };

  ggo::apply_vertical_processing<ggo::border_mode::zero>(input_image, output_image, 0, 1, processing);

  GGO_CHECK_PIXELS(output_image, {
    -5.f, -1.f, -2.f,
     3.f, -3.f,  1.f,
     0.f,  3.f, -3.f,
     1.f, -4.f,  1.f });
}

////////////////////////////////////////////////////////////////////
GGO_TEST(image_local_processing, vertical_symmetric)
{
  const auto input_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 3, 4 }, {
     5.f, 1.f, 2.f,
     2.f, 4.f, 1.f,
     2.f, 1.f, 4.f,
     1.f, 5.f, 3.f });
  auto output_image = make_image_rows_down_t<ggo::pixel_type::y_32f>({ 3, 4 }, 0.f);

  auto processing = [](const auto & neighborhood)
  {
    return neighborhood(-1) + neighborhood(0) + neighborhood(1);
  };

  ggo::apply_vertical_processing<ggo::border_mode::mirror>(input_image, output_image, 1, 1, processing);

  GGO_CHECK_PIXELS(output_image, {
    12.f,  6.f,  5.f,
     9.f,  6.f,  7.f,
     5.f, 10.f,  8.f,
     4.f, 11.f, 10.f });
}

