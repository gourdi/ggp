#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_forward_input_cache.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(forward_input, cache_border)
{
  ggo::forward_input_cache<int, ggo::border_mode::mirror> input(4, 3);

  input.setup([](int i) { return i; });

  GGO_CHECK_EQ(input.read(-2), 1);
  GGO_CHECK_EQ(input.read(-1), 0);
  GGO_CHECK_EQ(input.read(0), 0);
  GGO_CHECK_EQ(input.read(1), 1);
  GGO_CHECK_EQ(input.read(2), 2);

  input.move_to_next();
  GGO_CHECK_EQ(input.read(-2), 0);
  GGO_CHECK_EQ(input.read(-1), 0);
  GGO_CHECK_EQ(input.read(0), 1);
  GGO_CHECK_EQ(input.read(1), 2);
  GGO_CHECK_EQ(input.read(2), 3);

  input.move_to_next();
  GGO_CHECK_EQ(input.read(-2), 0);
  GGO_CHECK_EQ(input.read(-1), 1);
  GGO_CHECK_EQ(input.read(0), 2);
  GGO_CHECK_EQ(input.read(1), 3);
  GGO_CHECK_EQ(input.read(2), 3);

  input.move_to_next();
  GGO_CHECK_EQ(input.read(-2), 1);
  GGO_CHECK_EQ(input.read(-1), 2);
  GGO_CHECK_EQ(input.read(0), 3);
  GGO_CHECK_EQ(input.read(1), 3);
  GGO_CHECK_EQ(input.read(2), 2);
}
