#include <kernel/ggo_borders.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, loop_index)
{
  static_assert(ggo::loop_index(-6, 4) == 2);
  static_assert(ggo::loop_index(-5, 4) == 3);
  static_assert(ggo::loop_index(-4, 4) == 0);
  static_assert(ggo::loop_index(-3, 4) == 1);
  static_assert(ggo::loop_index(-2, 4) == 2);
  static_assert(ggo::loop_index(-1, 4) == 3);
  static_assert(ggo::loop_index(0, 4) == 0);
  static_assert(ggo::loop_index(1, 4) == 1);
  static_assert(ggo::loop_index(2, 4) == 2);
  static_assert(ggo::loop_index(3, 4) == 3);
  static_assert(ggo::loop_index(4, 4) == 0);
  static_assert(ggo::loop_index(5, 4) == 1);
  static_assert(ggo::loop_index(6, 4) == 2);
  static_assert(ggo::loop_index(7, 4) == 3);
  static_assert(ggo::loop_index(8, 4) == 0);
  static_assert(ggo::loop_index(9, 4) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, mirror_index)
{
  static_assert(ggo::mirror_index(-6, 4) == 2);
  static_assert(ggo::mirror_index(-5, 4) == 3);
  static_assert(ggo::mirror_index(-4, 4) == 3);
  static_assert(ggo::mirror_index(-3, 4) == 2);
  static_assert(ggo::mirror_index(-2, 4) == 1);
  static_assert(ggo::mirror_index(-1, 4) == 0);
  static_assert(ggo::mirror_index(0, 4) == 0);
  static_assert(ggo::mirror_index(1, 4) == 1);
  static_assert(ggo::mirror_index(2, 4) == 2);
  static_assert(ggo::mirror_index(3, 4) == 3);
  static_assert(ggo::mirror_index(4, 4) == 3);
  static_assert(ggo::mirror_index(5, 4) == 2);
  static_assert(ggo::mirror_index(6, 4) == 1);
  static_assert(ggo::mirror_index(7, 4) == 0);
  static_assert(ggo::mirror_index(8, 4) == 0);
  static_assert(ggo::mirror_index(9, 4) == 1);
}

