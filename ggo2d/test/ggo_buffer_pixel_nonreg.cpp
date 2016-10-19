#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <ggo_pixel_buffer.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, y_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::y_8u_yu>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 20);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 27);

  auto c = ggo::get_pixel<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10);
  GGO_CHECK_EQ(c, 27);

  ggo::set_pixel<ggo::y_8u_yu>(buffer.data(), 6, 0, 2, 10, { 30 });
  ggo::set_pixel<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10, { 40 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 30, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 40, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, y_32f_yu)
{
  std::vector<float> buffer{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f };

  const float * line_ptr = static_cast<const float*>(ggo::get_line_ptr<ggo::y_32f_yu>(buffer.data(), 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FABS(*line_ptr, 20.f);

  const float * pixel_ptr = static_cast<const float*>(ggo::get_pixel_ptr<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FABS(*pixel_ptr, 27.f);

  auto c = ggo::get_pixel<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float));
  GGO_CHECK_FABS(c, 27.f);

  ggo::set_pixel<ggo::y_32f_yu>(buffer.data(), 6, 0, 2, 10 * sizeof(float), { 30 });
  ggo::set_pixel<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float), { 40 });

  const std::vector<float> expected{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 30.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 40.f, 28.f, 29.f };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, rgb_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::rgb_8u_yu>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 20);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::rgb_8u_yu>(buffer.data(), 2, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 26);

  auto c = ggo::get_pixel<ggo::rgb_8u_yu>(buffer.data(), 2, 1, 2, 10);
  GGO_CHECK_EQ(26, c._r);
  GGO_CHECK_EQ(27, c._g);
  GGO_CHECK_EQ(28, c._b);

  ggo::set_pixel<ggo::rgb_8u_yu>(buffer.data(), 2, 0, 2, 10, { 30, 31, 32 });
  ggo::set_pixel<ggo::rgb_8u_yu>(buffer.data(), 1, 1, 2, 10, { 40, 41, 42 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 30, 31, 32, 19,
    20, 21, 22, 40, 41, 42, 26, 27, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, rgb_32f_yu)
{
  std::vector<float> buffer{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f };

  const float * line_ptr = static_cast<const float*>(ggo::get_line_ptr<ggo::rgb_32f_yu>(buffer.data(), 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FABS(*line_ptr, 20.f);

  const float * pixel_ptr = static_cast<const float*>(ggo::get_pixel_ptr<ggo::rgb_32f_yu>(buffer.data(), 2, 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FABS(*pixel_ptr, 26.f);

  auto c = ggo::get_pixel<ggo::rgb_32f_yu>(buffer.data(), 2, 1, 2, 10 * sizeof(float));
  GGO_CHECK_FABS(c._r, 26.f);
  GGO_CHECK_FABS(c._g, 27.f);
  GGO_CHECK_FABS(c._b, 28.f);

  ggo::set_pixel<ggo::rgb_32f_yu>(buffer.data(), 2, 0, 2, 10 * sizeof(float), { 30.f, 31.f, 32.f });
  ggo::set_pixel<ggo::rgb_32f_yu>(buffer.data(), 1, 1, 2, 10 * sizeof(float), { 40.f, 41.f, 42.f });

  const std::vector<float> expected{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 30.f, 31.f, 32.f, 19.f,
    20.f, 21.f, 22.f, 40.f, 41.f, 42.f, 26.f, 27.f, 28.f, 29.f };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, bgra_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::bgra_8u_yd>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 10);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 14);

  auto c = ggo::get_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10);
  GGO_CHECK_EQ(16, c._r);
  GGO_CHECK_EQ(15, c._g);
  GGO_CHECK_EQ(14, c._b);

  ggo::set_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 0, 2, 10, { 30, 31, 32 });
  ggo::set_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10, { 40, 41, 42 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 42, 41, 40, 17, 18, 19,
    20, 21, 22, 23, 32, 31, 30, 27, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_pixel, accumulator)
{
  {
    ggo::accumulator<uint8_t> acc;
    acc.add(3);
    acc.add(4);
    GGO_CHECK_EQ(acc.acc, 7);
    GGO_CHECK_EQ(acc.div<8>(), 1);
  }

  {
    ggo::accumulator<float> acc;
    acc.add(3.f);
    acc.add(4.f);
    GGO_CHECK_FABS(acc.acc, 7.f);
    GGO_CHECK_FABS(acc.div<8>(), 7.f / 8.f);
  }

  {
    ggo::accumulator<ggo::color_8u> acc;
    acc.add({ 2, 0, 1 });
    acc.add({ 2, 0, 1 });
    acc.add({ 2, 3, 1 });
    acc.add({ 1, 0, 2 });
    auto color = acc.div<4>();
    GGO_CHECK_EQ(acc.r, 7);
    GGO_CHECK_EQ(acc.g, 3);
    GGO_CHECK_EQ(acc.b, 5);
    GGO_CHECK_EQ(color._r, 2);
    GGO_CHECK_EQ(color._g, 1);
    GGO_CHECK_EQ(color._b, 1);
  }

  {
    ggo::accumulator<ggo::color_32f> acc;
    acc.add({ 2.f, 0.f, 1.f });
    acc.add({ 2.f, 0.f, 1.f });
    acc.add({ 2.f, 3.f, 1.f });
    acc.add({ 1.f, 0.f, 2.f });
    auto color = acc.div<4>();
    GGO_CHECK_EQ(acc.r, 7.f);
    GGO_CHECK_EQ(acc.g, 3.f);
    GGO_CHECK_EQ(acc.b, 5.f);
    GGO_CHECK_EQ(color._r, 7.f / 4.f);
    GGO_CHECK_EQ(color._g, 3.f / 4.f);
    GGO_CHECK_EQ(color._b, 5.f / 4.f);
  }
}
