#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, clipping)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  const ggo::buffer<uint8_t> expected(dst_width * dst_height, 0);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, -3, 0);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 6, 0);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 0, -2);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 0, 5);
  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_fit)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 2, 1);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_bottom_left)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, -2, -1);

  const ggo::buffer<uint8_t> expected{
    6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_top_right)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 4, 4);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 2 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_overlap)
{
  const int dst_width = 3;
  const int dst_height = 2;
  ggo::buffer<uint8_t> dst(dst_width * dst_height);

  const int src_width = 6;
  const int src_height = 4;
  const ggo::buffer<uint8_t> src{
    11, 12, 13, 14, 15, 16,
    21, 22, 23, 24, 25, 26, 
    31, 32, 33, 34, 35, 36, 
    41, 42, 43, 44, 45, 46 };

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src_width, src_height, src_width,
      dst.data(), dst_width, dst_height, dst_width, -2, -1);

    const ggo::buffer<uint8_t> expected{
      23, 24, 25,
      33, 34, 35 };

    GGO_CHECK(dst == expected);
  }

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src_width, src_height, src_width,
      dst.data(), dst_width, dst_height, dst_width, -5, -1);

    const ggo::buffer<uint8_t> expected{
      26, 0, 0,
      36, 0, 0 };

    GGO_CHECK(dst == expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_fit)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 2, 1);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_bottom_left)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, -2, -1);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_top_right)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 4, 4);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 4, 5,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_overlap)
{
  const int dst_width = 3;
  const int dst_height = 2;
  ggo::buffer<uint8_t> dst(dst_width * dst_height);

  const int src_width = 6;
  const int src_height = 4;
  const ggo::buffer<uint8_t> src{
    11, 12, 13, 14, 15, 16,
    21, 22, 23, 24, 25, 26,
    31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46 };

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
      src.data(), src_width, src_height, src_width,
      dst.data(), dst_width, dst_height, dst_width, -2, -1);

    const ggo::buffer<uint8_t> expected{
      23, 24, 25,
      33, 34, 35 };

    GGO_CHECK(dst == expected);
  }

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src_width, src_height, src_width,
      dst.data(), dst_width, dst_height, dst_width, -5, -1);

    const ggo::buffer<uint8_t> expected{
      26, 0, 0,
      36, 0, 0 };

    GGO_CHECK(dst == expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_down_src_up_fit)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yd>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 2, 1);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_up_src_down_fit)
{
  const int dst_width = 6;
  const int dst_height = 5;
  ggo::buffer<uint8_t> dst(dst_width * dst_height, 0);

  const int src_width = 3;
  const int src_height = 2;
  const ggo::buffer<uint8_t> src{ 1, 2, 3, 4, 5, 6 };

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yu>(
    src.data(), src_width, src_height, src_width,
    dst.data(), dst_width, dst_height, dst_width, 2, 1);

  const ggo::buffer<uint8_t> expected{
    0, 0, 0, 0, 0, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0 };

  GGO_CHECK(dst == expected);
}


