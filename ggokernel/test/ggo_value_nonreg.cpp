#include "ggo_kernel_nonreg.h"
#include <ggo_value.h>
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
    ggo::value v(ggo::pos2f(1.f, 2.f));
    GGO_CHECK_FLOAT_EQ(v.to<ggo::pos2f>().x(), 1.f);
    GGO_CHECK_FLOAT_EQ(v.to<ggo::pos2f>().y(), 2.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(value, array)
{
  {
    std::array<int, 3> a{ 1, 2, 3 };
    ggo::value val(a.begin(), a.end());

    int i = 0;
    for (auto v : val.to_range_of<int>())
    {
      GGO_CHECK_EQ(v, a[i++]);
    }
  }
  {
    std::list<ggo::pos2f> a{ { 1.f, 2.f },{ 3.f, 4.f } };
    ggo::value val(a.begin(), a.end());

    int i = 0;
    for (auto v : val.to_range_of<ggo::pos2f>())
    {
      GGO_CHECK_FLOAT_EQ(v.x(), i == 0 ? 1.f : 3.0f);
      GGO_CHECK_FLOAT_EQ(v.y(), i == 0 ? 2.f : 4.0f);

      ++i;
    }
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

