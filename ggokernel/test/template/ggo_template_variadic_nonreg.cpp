#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>

#if 0

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_variadic, accumulate)
{
  auto f = [](int v1, int v2) { return std::abs(v1 - 5) < std::abs(v2 - 5) ? v1 : v2; };
  GGO_CHECK_EQ(ggo::accumulate(f, 2, 6), 6);
  GGO_CHECK_EQ(ggo::accumulate(f, 6, 2), 6);
  GGO_CHECK_EQ(ggo::accumulate(f, 2, 6, 8), 6);
  GGO_CHECK_EQ(ggo::accumulate(f, 8, 9, 4), 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_variadic, min_max)
{
  GGO_CHECK(ggo::min(1, 2) == 1);
  GGO_CHECK(ggo::min(1, 2, 5) == 1);
  GGO_CHECK(ggo::min(5, 2, 1) == 1);
  GGO_CHECK(ggo::min(-1, 2, 5, 8, 9) == -1);

  GGO_CHECK(ggo::max(1, 2) == 2);
  GGO_CHECK(ggo::max(1, 2, 5) == 5);
  GGO_CHECK(ggo::max(5, 2, 1) == 5);
  GGO_CHECK(ggo::max(-1, 2, 5, 8, 9) == 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_variadic, sum)
{
  constexpr int s1 = ggo::sum(1, 2, 3);
  static_assert(s1 == 6);

  float s2 = ggo::sum(0.1f, 0.2f, 0.3f, 0.4f);
  GGO_CHECK_FLOAT_EQ(s2, 1.f);

  constexpr uint8_t v1 = 0xFF;
  constexpr uint8_t v2 = 0xFF;
  constexpr uint8_t v3 = 0xFF;
  constexpr uint8_t v4 = 0xFF;
  static_assert(ggo::sum_to<int>(v1, v2, v3, v4) == 1020);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(template_variadic, average)
{
  float a1 = ggo::average(1.f, 2.f, 3.f);
  GGO_CHECK_FLOAT_EQ(a1, 2.f);

  float a2 = ggo::average(1.f, 2.f);
  GGO_CHECK_FLOAT_EQ(a2, 1.5f);

  uint8_t v1 = 1;
  uint8_t v2 = 1;
  uint8_t v3 = 1;
  uint8_t v4 = 0;
  uint8_t a3 = ggo::average(v1, v2, v3, v4);
  GGO_CHECK_EQ(a3, 1);

  v1 = 0xFF;
  v2 = 0xFF;
  v3 = 0xFF;
  v4 = 0xFF;
  int a4 = ggo::average(v1, v2, v3, v4);
  GGO_CHECK_EQ(a4, 0xFF);

  int a5 = ggo::average(-1, -1, -1, 0);
  GGO_CHECK_EQ(a5, -1);
}

#endif
