#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_ratio.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, equality)
{
  static_assert(ggo::ratio{ 1, 2 } == ggo::ratio{ 2, 4 });
  static_assert(ggo::ratio{ 2, 7 } == ggo::ratio{ 6, 21 });
  static_assert(ggo::ratio{ 6, 2 } == 3);

  static_assert(ggo::ratio{ 1, 2 } != ggo::ratio{ 2, 3 });
  static_assert(ggo::ratio{ 1, 2 } != ggo::ratio{ 1, 4 });
  static_assert(ggo::ratio{ 2, 7 } != ggo::ratio{ 6, 20 });
  static_assert(ggo::ratio{ 2, 7 } != ggo::ratio{ 5, 21 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, multiply)
{
  static_assert(ggo::ratio{ 1, 2 } * ggo::ratio{ 2, 3 } == ggo::ratio{ 1, 3 });
  static_assert(3 * ggo::ratio{ 1, 2 } == ggo::ratio{ 3, 2 });
  static_assert(ggo::ratio{ 1, 2 } * 3 == ggo::ratio{ 3, 2 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, divide)
{
  static_assert(ggo::ratio{ 5, 7 } / ggo::ratio{ 2, 3 } == ggo::ratio{ 15, 14 });
  static_assert(3 / ggo::ratio{ 1, 2 } == ggo::ratio{ 6, 1 });
  static_assert(ggo::ratio{ 3, 2 } / 3 == ggo::ratio{ 1, 2 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, add_same_denominator)
{
  constexpr auto res = ggo::ratio{ 1, 4 } + ggo::ratio{ 2, 4 };
  static_assert(res._num == 3);
  static_assert(res._den == 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, add_different_denominator)
{
  constexpr auto res = ggo::ratio{ 1, 4 } + ggo::ratio{ 2, 3 };
  static_assert(res._num == 11);
  static_assert(res._den == 12);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, sub_same_denominator)
{
  constexpr auto res = ggo::ratio{ 13, 4 } - ggo::ratio{ 11, 4 };
  static_assert(res._num == 2);
  static_assert(res._den == 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, sub_different_denominator)
{
  constexpr auto res = ggo::ratio{ 2, 3 } - ggo::ratio{ 1, 4 };
  static_assert(res._num == 5);
  static_assert(res._den == 12);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, comparison)
{
  static_assert(ggo::ratio{ 5, 7 } > ggo::ratio{ 1, 2 });
  static_assert((ggo::ratio{ 5, 7 } > ggo::ratio{ 5, 7 }) == false);
  static_assert((ggo::ratio{ 1, 2 } > ggo::ratio{ 5, 7 }) == false);

  static_assert(ggo::ratio{ 5, 7 } >= ggo::ratio{ 1, 2 });
  static_assert(ggo::ratio{ 5, 7 } >= ggo::ratio{ 5, 7 });
  static_assert((ggo::ratio{ 1, 2 } >= ggo::ratio{ 5, 7 }) == false);

  static_assert((ggo::ratio{ 5, 7 } <= ggo::ratio{ 1, 2 }) == false);
  static_assert(ggo::ratio{ 5, 7 } <= ggo::ratio{ 5, 7 });
  static_assert(ggo::ratio{ 1, 2 } <= ggo::ratio{ 5, 7 });

  static_assert((ggo::ratio{ 5, 7 } < ggo::ratio{ 1, 2 }) == false);
  static_assert((ggo::ratio{ 5, 7 } < ggo::ratio{ 5, 7 }) == false);
  static_assert(ggo::ratio{ 1, 2 } < ggo::ratio{ 5, 7 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ratio, to_float)
{
  static_assert(ggo::to<float>(ggo::ratio{ 5, 4 }) == 1.25f);
}
