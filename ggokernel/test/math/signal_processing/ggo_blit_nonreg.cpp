#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_processing/ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 1d_no_clipping)
{
  std::vector<int> dst{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const std::vector<int> src{ 10, 11, 12, 13, 14 };

  ggo::blit([&](int x, int v) { dst[x] = v; }, int(dst.size()), [&](int x) { return src[x]; }, int(src.size()), 2);

  const std::vector<int> expected{ 0, 1, 10, 11, 12, 13, 14, 7, 8, 9 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 1d_right_clipping)
{
  std::vector<int> dst{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const std::vector<int> src{ 10, 11, 12, 13, 14 };

  ggo::blit([&](int x, int v) { dst[x] = v; }, int(dst.size()), [&](int x) { return src[x]; }, int(src.size()), 8);

  const std::vector<int> expected{ 0, 1, 2, 3, 4, 5, 6, 7, 10, 11 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 1d_left_clipping)
{
  std::vector<int> dst{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const std::vector<int> src{ 10, 11, 12, 13, 14 };

  ggo::blit([&](int x, int v) { dst[x] = v; }, int(dst.size()), [&](int x) { return src[x]; }, int(src.size()), -2);

  const std::vector<int> expected{ 12, 13, 14, 3, 4, 5, 6, 7, 8, 9 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 1d_full_overlap)
{
  std::vector<int> dst{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const std::vector<int> src{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };

  ggo::blit([&](int x, int v) { dst[x] = v; }, int(dst.size()), [&](int x) { return src[x]; }, int(src.size()), -2);

  const std::vector<int> expected{ 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 2d_no_clipping)
{
  std::vector<int> dst{ 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  const std::vector<int> src{ 
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34,
    40, 41, 42, 43, 44 };

  ggo::blit([&](int x, int y, int v) { dst[y * 10 + x] = v; }, { 10, 6 }, [&](int x, int y) { return src[y * 5 + x]; }, { 5, 4 }, { 3, 1 });

  const std::vector<int> expected{ 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 10, 11, 12, 13, 14, 28, 29,
    30, 31, 32, 20, 21, 22, 23, 24, 38, 39,
    40, 41, 42, 30, 31, 32, 33, 34, 48, 49,
    50, 51, 52, 40, 41, 42, 43, 44, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 2d_clipping_top_right)
{
  std::vector<int> dst{ 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  const std::vector<int> src{ 
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34,
    40, 41, 42, 43, 44 };

  ggo::blit([&](int x, int y, int v) { dst[y * 10 + x] = v; }, { 10, 6 }, [&](int x, int y) { return src[y * 5 + x]; }, { 5, 4 }, { 7, 4 });

  const std::vector<int> expected{ 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 10, 11, 12,
    60, 61, 62, 63, 64, 65, 66, 20, 21, 22 };
  GGO_CHECK_EQ(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, 2d_clipping_bottom_left)
{
  std::vector<int> dst{ 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  const std::vector<int> src{ 
    10, 11, 12, 13, 14,
    20, 21, 22, 23, 24,
    30, 31, 32, 33, 34,
    40, 41, 42, 43, 44 };

  ggo::blit([&](int x, int y, int v) { dst[y * 10 + x] = v; }, { 10, 6 }, [&](int x, int y) { return src[y * 5 + x]; }, { 5, 4 }, { -2, -1 });

  const std::vector<int> expected{ 
    22, 23, 24, 13, 14, 15, 16, 17, 18, 19,
    32, 33, 34, 23, 24, 25, 26, 27, 28, 29,
    42, 43, 44, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69 };
  GGO_CHECK_EQ(dst, expected);
}




