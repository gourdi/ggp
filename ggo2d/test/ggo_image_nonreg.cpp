#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <2d/ggo_image.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, y_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::y_8u_yu>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 20);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 27);

  auto c = ggo::read_pixel<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10);
  GGO_CHECK_EQ(c, 27);

  ggo::write_pixel<ggo::y_8u_yu>(buffer.data(), 6, 0, 2, 10, { 30 });
  ggo::write_pixel<ggo::y_8u_yu>(buffer.data(), 7, 1, 2, 10, { 40 });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 30, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 40, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, y_32f_yu)
{
  std::vector<float> buffer{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f };

  const float * line_ptr = static_cast<const float*>(ggo::get_line_ptr<ggo::y_32f_yu>(buffer.data(), 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FLOAT_EQ(*line_ptr, 20.f);

  const float * pixel_ptr = static_cast<const float*>(ggo::get_pixel_ptr<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FLOAT_EQ(*pixel_ptr, 27.f);

  auto c = ggo::read_pixel<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float));
  GGO_CHECK_FLOAT_EQ(c, 27.f);

  ggo::write_pixel<ggo::y_32f_yu>(buffer.data(), 6, 0, 2, 10 * sizeof(float), { 30 });
  ggo::write_pixel<ggo::y_32f_yu>(buffer.data(), 7, 1, 2, 10 * sizeof(float), { 40 });

  const std::vector<float> expected{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 30.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 40.f, 28.f, 29.f };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, rgb_8u_yu)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::rgb_8u_yu>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 20);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::rgb_8u_yu>(buffer.data(), 2, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 26);

  auto c = ggo::read_pixel<ggo::rgb_8u_yu>(buffer.data(), 2, 1, 2, 10);
  GGO_CHECK_EQ(26, c.r());
  GGO_CHECK_EQ(27, c.g());
  GGO_CHECK_EQ(28, c.b());

  ggo::write_pixel<ggo::rgb_8u_yu>(buffer.data(), 2, 0, 2, 10, { uint8_t(30), uint8_t(31), uint8_t(32) });
  ggo::write_pixel<ggo::rgb_8u_yu>(buffer.data(), 1, 1, 2, 10, { uint8_t(40), uint8_t(41), uint8_t(42) });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 14, 15, 30, 31, 32, 19,
    20, 21, 22, 40, 41, 42, 26, 27, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, rgb_32f_yu)
{
  std::vector<float> buffer{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f,
    20.f, 21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f };

  const float * line_ptr = static_cast<const float*>(ggo::get_line_ptr<ggo::rgb_32f_yu>(buffer.data(), 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FLOAT_EQ(*line_ptr, 20.f);

  const float * pixel_ptr = static_cast<const float*>(ggo::get_pixel_ptr<ggo::rgb_32f_yu>(buffer.data(), 2, 1, 2, 10 * sizeof(float)));
  GGO_CHECK_FLOAT_EQ(*pixel_ptr, 26.f);

  auto c = ggo::read_pixel<ggo::rgb_32f_yu>(buffer.data(), 2, 1, 2, 10 * sizeof(float));
  GGO_CHECK_FLOAT_EQ(c.r(), 26.f);
  GGO_CHECK_FLOAT_EQ(c.g(), 27.f);
  GGO_CHECK_FLOAT_EQ(c.b(), 28.f);

  ggo::write_pixel<ggo::rgb_32f_yu>(buffer.data(), 2, 0, 2, 10 * sizeof(float), { 30.f, 31.f, 32.f });
  ggo::write_pixel<ggo::rgb_32f_yu>(buffer.data(), 1, 1, 2, 10 * sizeof(float), { 40.f, 41.f, 42.f });

  const std::vector<float> expected{
    10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 30.f, 31.f, 32.f, 19.f,
    20.f, 21.f, 22.f, 40.f, 41.f, 42.f, 26.f, 27.f, 28.f, 29.f };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, bgra_8u_yd)
{
  std::vector<uint8_t> buffer{
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

  const uint8_t * line_ptr = static_cast<const uint8_t*>(ggo::get_line_ptr<ggo::bgra_8u_yd>(buffer.data(), 1, 2, 10));
  GGO_CHECK_EQ(*line_ptr, 10);

  const uint8_t * pixel_ptr = static_cast<const uint8_t*>(ggo::get_pixel_ptr<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10));
  GGO_CHECK_EQ(*pixel_ptr, 14);

  auto c = ggo::read_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10);
  GGO_CHECK_EQ(16, c.r());
  GGO_CHECK_EQ(15, c.g());
  GGO_CHECK_EQ(14, c.b());

  ggo::write_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 0, 2, 10, { uint8_t(30), uint8_t(31), uint8_t(32), uint8_t(33) });
  ggo::write_pixel<ggo::bgra_8u_yd>(buffer.data(), 1, 1, 2, 10, { uint8_t(40), uint8_t(41), uint8_t(42), uint8_t(43) });

  const std::vector<uint8_t> expected{
    10, 11, 12, 13, 42, 41, 40, 43, 18, 19,
    20, 21, 22, 23, 32, 31, 30, 33, 28, 29 };

  GGO_CHECK(buffer == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, accumulator)
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
    GGO_CHECK_FLOAT_EQ(acc.acc, 7.f);
    GGO_CHECK_FLOAT_EQ(acc.div<8>(), 7.f / 8.f);
  }

  {
    ggo::accumulator<ggo::color_8u> acc;
    acc.add({ uint8_t(2), uint8_t(0), uint8_t(1) });
    acc.add({ uint8_t(2), uint8_t(0), uint8_t(1) });
    acc.add({ uint8_t(2), uint8_t(3), uint8_t(1) });
    acc.add({ uint8_t(1), uint8_t(0), uint8_t(2) });
    auto color = acc.div<4>();
    GGO_CHECK_EQ(acc.r, 7);
    GGO_CHECK_EQ(acc.g, 3);
    GGO_CHECK_EQ(acc.b, 5);
    GGO_CHECK_EQ(color.r(), 2);
    GGO_CHECK_EQ(color.g(), 1);
    GGO_CHECK_EQ(color.b(), 1);
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
    GGO_CHECK_EQ(color.r(), 7.f / 4.f);
    GGO_CHECK_EQ(color.g(), 3.f / 4.f);
    GGO_CHECK_EQ(color.b(), 5.f / 4.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, move)
{
  ggo::image pb1(10, 30, 30, ggo::rgb_8u_yu);
  ggo::image pb2(10, 20, 30, ggo::rgb_8u_yu);

  pb1 = std::move(pb2);
}
