#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_curve.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(curve, linear)
{
  {
    ggo::linear_curve_float curve;

    curve.push_point(1.f, -1.f);
    curve.push_point(3.f, 3.f);

    GGO_CHECK_FLOAT_EQ(curve.evaluate(0.f), -1.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(1.f), -1.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(2.f), 1.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(3.f), 3.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(4.f), 3.f);
  }

  {
    ggo::linear_curve_float curve;

    curve.push_point(0.f, 0.f);
    curve.push_point(1.f, 1.f);
    curve.push_point(2.f, 3.f);

    GGO_CHECK_FLOAT_EQ(curve.evaluate(-0.5f), 0.0f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(0.0f), 0.0f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(0.5f), 0.5f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(1.0f), 1.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(1.5f), 2.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(2.0f), 3.f);
    GGO_CHECK_FLOAT_EQ(curve.evaluate(2.5f), 3.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(curve, cubic)
{
  ggo::cubic_curve_float curve;

  curve.push_point(1.f, -1.f);
  curve.push_point(2.f, 3.f);
  curve.push_point(3.f, 2.f);
 
  GGO_CHECK_FLOAT_EQ(curve.evaluate(0.9f), -1.000000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.0f), -1.000000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.1f), -0.901501f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.2f), -0.632004f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.3f), -0.230499f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.4f),  0.264000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.5f),  0.812500f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.6f),  1.376000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.7f),  1.915500f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.8f),  2.392000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(1.9f),  2.766500f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.0f),  3.000000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.1f),  3.093510f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.2f),  3.088000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.3f),  3.004520f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.4f),  2.863980f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.5f),  2.687500f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.6f),  2.496000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.7f),  2.310500f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.8f),  2.152010f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(2.9f),  2.041490f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(3.0f),  2.000000f);
  GGO_CHECK_FLOAT_EQ(curve.evaluate(3.1f),  2.000000f);
}


