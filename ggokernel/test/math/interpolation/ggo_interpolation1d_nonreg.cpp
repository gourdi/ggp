#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, cubic)
{
  auto cub1 = ggo::cubic<float, float>::make_smooth(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f);
  GGO_CHECK_FLOAT_EQ(cub1._a, 2.f);
  GGO_CHECK_FLOAT_EQ(cub1._b, -3.f);
  GGO_CHECK_FLOAT_EQ(cub1._c, 0.f);
  GGO_CHECK_FLOAT_EQ(cub1._d, 1.f);
  GGO_CHECK_FLOAT_EQ(cub1.evaluate(0.f), 1.f);
  GGO_CHECK_FLOAT_EQ(cub1.evaluate(1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub1.derivate(0.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub1.derivate(1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub1.integrate(0.f, 1.f), 0.5f);

  auto cub2 = ggo::cubic<float, float>::make_smooth(0.f, 1.f, 0.f, 1.f);
  GGO_CHECK_FLOAT_EQ(cub2._a, 2.f);
  GGO_CHECK_FLOAT_EQ(cub2._b, -3.f);
  GGO_CHECK_FLOAT_EQ(cub2._c, 0.f);
  GGO_CHECK_FLOAT_EQ(cub2._d, 1.f);
  GGO_CHECK_FLOAT_EQ(cub2.evaluate(0.f), 1.f);
  GGO_CHECK_FLOAT_EQ(cub2.evaluate(1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub2.derivate(0.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub2.derivate(1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub2.integrate(0.f, 1.f), 0.5f);

  auto cub3 = ggo::cubic<float, float>::make_smooth(-2.f, 1.f, 1.f, 4.f, 2.f, 3.f, 6.f, 4.f);
  GGO_CHECK_FLOAT_EQ(cub3._a, 2.5f);
  GGO_CHECK_FLOAT_EQ(cub3._b, -11.5f);
  GGO_CHECK_FLOAT_EQ(cub3._c, 16.f);
  GGO_CHECK_FLOAT_EQ(cub3._d, -3.f);
  GGO_CHECK_FLOAT_EQ(cub3.evaluate(1.f), 4.f);
  GGO_CHECK_FLOAT_EQ(cub3.evaluate(2.f), 3.f);
  GGO_CHECK_FLOAT_EQ(cub3.derivate(1.f), 0.5f);
  GGO_CHECK_FLOAT_EQ(cub3.derivate(2.f), 0.f);
  GGO_CHECK_FLOAT_EQ(cub3.integrate(1.f, 2.f), 3.54167f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, cubic_interpolation1)
{
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.0f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.1f), 0.972f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.2f), 0.896f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.3f), 0.784f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.4f), 0.648f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.5f), 0.5f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.6f), 0.352f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.7f), 0.216f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.8f), 0.104f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 0.9f), 0.028f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f, 1.0f), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, optimized_cubic_interpolation1)
{
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.0f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.1f), 0.972f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.2f), 0.896f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.3f), 0.784f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.4f), 0.648f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.5f), 0.5f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.6f), 0.352f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.7f), 0.216f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.8f), 0.104f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 0.9f), 0.028f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(0.f, 1.f, 0.f, 1.f, 1.0f), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation1d, optimized_cubic_interpolation2)
{
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.0f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.1f), 0.9875f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.2f), 1.04f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.3f), 1.1425f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.4f), 1.28f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.5f), 1.4375f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.6f), 1.6f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.7f), 1.7525f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.8f), 1.88f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 0.9f), 1.9675f);
  GGO_CHECK_FLOAT_EQ(ggo::cubic_interpolation(3.f, 1.f, 2.f, 1.f, 1.0f), 2.f);
}
