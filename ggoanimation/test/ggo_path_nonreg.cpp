#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <animation/paths/ggo_velocity_path.h>
#include <animation/paths/ggo_spiral_path.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(path, velocity)
{
  ggo::velocity_path path(ggo::vec2_f(1.f, 2.f));

  const auto p1 = path.get_pos(0);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.f);

  const auto p2 = path.get_pos(10);
  GGO_CHECK_FLOAT_EQ(p2.x(), 10.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 20.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(path, spiral)
{
  ggo::spiral_path path(ggo::pi<float>() / 2, 1.f);

  const auto p1 = path.get_pos(0);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.f);

  const auto p2 = path.get_pos(2);
  GGO_CHECK_FLOAT_EQ(p2.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.f);
}
