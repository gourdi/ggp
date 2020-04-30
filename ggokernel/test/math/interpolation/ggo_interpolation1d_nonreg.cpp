#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, linear_fixed_step)
{
  const float data[] = { 1, 2, 7, 5 };

  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 0.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 1.0f), 2.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 2.0f), 7.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 3.0f), 5.0f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 0.5f), 1.5f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_interpolation1d(data, 2.5f), 6.0f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, linear_variable_step)
{
  const std::map<float, float> data({ { 1.f, 1.f }, { 3.f, 0.f }, { 4.f, 2.f } });

  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 0.0f), 1.0f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 1.0f), 1.0f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 2.0f), 0.5f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 3.0f), 0.0f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 3.5f), 1.0f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 4.0f), 2.0f);
  GGO_CHECK_EQ(ggo::linear_interpolation1d(data.begin(), data.end(), 5.0f), 2.0f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, cubic_fixed_step)
{
  const float data[] = { 1, 2, 7, 5 };

  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 0.0f), 1.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 1.0f), 2.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 2.0f), 7.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 3.0f), 5.0f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 0.5f), 1.1875f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data, 4, 2.5f), 6.3125f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, cubic_variable_step)
{
  const std::vector<std::pair<float, float>> data({ { 1.f, -1.f }, { 2.f, 3.f }, { 3.f, 2.f } });

  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 0.9f), -1.000000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.0f), -1.000000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.1f), -0.901501f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.2f), -0.632004f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.3f), -0.230499f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.4f), 0.264000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.5f), 0.812500f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.6f), 1.376000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.7f), 1.915500f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.8f), 2.392000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 1.9f), 2.766500f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.0f), 3.000000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.1f), 3.093510f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.2f), 3.088000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.3f), 3.004520f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.4f), 2.863980f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.5f), 2.687500f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.6f), 2.496000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.7f), 2.310500f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.8f), 2.152010f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 2.9f), 2.041490f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 3.0f), 2.000000f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation1d(data.begin(), data.end(), 3.1f), 2.000000f);
}



