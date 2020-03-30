#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_integration1d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration, linear)
{
  const float data[4] = { 0, 1, 1, 2 };

  GGO_CHECK_EQ(ggo::linear_integration1d(data, 1.0f, 2.0f), 1.0f);
  GGO_CHECK_EQ(ggo::linear_integration1d(data, 0.0f, 1.5f), 1.0f);
  GGO_CHECK_EQ(ggo::linear_integration1d(data, 0.0f, 2.0f), 1.5f);
  GGO_CHECK_EQ(ggo::linear_integration1d(data, 0.0f, 3.0f), 3.0f);
  GGO_CHECK_EQ(ggo::linear_integration1d(data, 2.25f, 2.75f), 0.75f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration, cubic)
{
  const float data[5] = { 0, 1, 0, -1, 0 };

  GGO_CHECK_FLOAT_EQ(ggo::cubic_integration1d(data, 5, 1.f, 3.f), 0.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_integration1d(data, 5, 2.25f, 2.75f), -0.307292f);
  GGO_CHECK_EQ(ggo::cubic_integration1d(data, 5, 0.f, 2.f), -ggo::cubic_integration1d(data, 5, 2.f, 4.f));
}

