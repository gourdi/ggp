#include "ggo_kernel_nonreg.h"
#include <kernel/ggo_value.h>
#include <list>

/////////////////////////////////////////////////////////////////////
GGO_TEST(value, basic)
{
  {
    ggo::value v(1.f);
    GGO_CHECK_FLOAT_EQ(v.to<float>(), 1.f);
  }
  {
    ggo::value v(1);
    GGO_CHECK_FLOAT_EQ(v.to<int>(), 1);
  }
  {
    ggo::value v(ggo::pos2_f(1.f, 2.f));
    GGO_CHECK_FLOAT_EQ(v.to<ggo::pos2_f>().x(), 1.f);
    GGO_CHECK_FLOAT_EQ(v.to<ggo::pos2_f>().y(), 2.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(value, type_mismatch_should_throw)
{
  ggo::value v(1);
  bool exception_caught = false;
  try
  {
    v.to<float>();
  }
  catch (...)
  {
    exception_caught = true;
  }
  GGO_CHECK(exception_caught);
}

