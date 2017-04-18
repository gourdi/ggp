#include "ggo_kernel_nonreg.h"
#include <ggo_value.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(value, basic)
{
  {
    ggo::value v(1.f);
    GGO_CHECK_FABS(v.to<float>(), 1.f);
  }
  {
    ggo::value v(1);
    GGO_CHECK_FABS(v.to<int>(), 1);
  }
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
}
