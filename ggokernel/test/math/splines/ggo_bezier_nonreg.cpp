#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_bezier.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(bezier, quadratic)
{
  GGO_CHECK_VEC2F_EQ(ggo::quadratic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, 0.0f), ggo::pos2_f(0.f, 0.f));
  GGO_CHECK_VEC2F_EQ(ggo::quadratic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, 1.0f), ggo::pos2_f(1.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::quadratic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, 0.5f), ggo::pos2_f(0.25f, 0.75f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bezier, cubic)
{
  GGO_CHECK_VEC2F_EQ(ggo::cubic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, 0.0f), ggo::pos2_f(0.f, 0.f));
  GGO_CHECK_VEC2F_EQ(ggo::cubic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, 1.0f), ggo::pos2_f(1.f, 0.f));
  GGO_CHECK_VEC2F_EQ(ggo::cubic_bezier<ggo::pos2_f>({ 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, 0.5f), ggo::pos2_f(0.5f, 0.75f));
}


