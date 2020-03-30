#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_interpolation_polynom.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation_polynom, linear)
{
  auto lin1 = ggo::make_interpolation_linear(4.f, 3.f, 6.f, 4.f);
  GGO_CHECK_FLOAT_EQ(lin1._coefs[1], 0.5f);
  GGO_CHECK_FLOAT_EQ(lin1._coefs[0], 1.0f);
  GGO_CHECK_FLOAT_EQ(evaluate(lin1, 4.f), 3.f);
  GGO_CHECK_FLOAT_EQ(evaluate(lin1, 6.f), 4.f);

  auto lin2 = ggo::make_interpolation_linear(1.f, 3.f);
  GGO_CHECK_FLOAT_EQ(lin2._coefs[1], 2.f);
  GGO_CHECK_FLOAT_EQ(lin2._coefs[0], 1.0f);
  GGO_CHECK_FLOAT_EQ(evaluate(lin2, 0.f), 1.f);
  GGO_CHECK_FLOAT_EQ(evaluate(lin2, 1.f), 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(interpolation_polynom, cubic)
{
  auto cub1 = ggo::make_interpolation_cubic(-1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 2.f, 1.f);
  GGO_CHECK_FLOAT_EQ(cub1._coefs[3], 2.f);
  GGO_CHECK_FLOAT_EQ(cub1._coefs[2], -3.f);
  GGO_CHECK_FLOAT_EQ(cub1._coefs[1], 0.f);
  GGO_CHECK_FLOAT_EQ(cub1._coefs[0], 1.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub1, 0.f), 1.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub1, 1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(derivate(cub1, 0.f), 0.f);
  GGO_CHECK_FLOAT_EQ(derivate(cub1, 1.f), 0.f);

  auto cub2 = ggo::make_interpolation_cubic(0.f, 1.f, 0.f, 1.f);
  GGO_CHECK_FLOAT_EQ(cub2._coefs[3], 2.f);
  GGO_CHECK_FLOAT_EQ(cub2._coefs[2], -3.f);
  GGO_CHECK_FLOAT_EQ(cub2._coefs[1], 0.f);
  GGO_CHECK_FLOAT_EQ(cub2._coefs[0], 1.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub2, 0.f), 1.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub2, 1.f), 0.f);
  GGO_CHECK_FLOAT_EQ(derivate(cub2, 0.f), 0.f);
  GGO_CHECK_FLOAT_EQ(derivate(cub2, 1.f), 0.f);

  auto cub3 = ggo::make_interpolation_cubic(-2.f, 1.f, 1.f, 4.f, 2.f, 3.f, 6.f, 4.f);
  GGO_CHECK_FLOAT_EQ(cub3._coefs[3], 2.5f);
  GGO_CHECK_FLOAT_EQ(cub3._coefs[2], -11.5f);
  GGO_CHECK_FLOAT_EQ(cub3._coefs[1], 16.f);
  GGO_CHECK_FLOAT_EQ(cub3._coefs[0], -3.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub3, 1.f), 4.f);
  GGO_CHECK_FLOAT_EQ(evaluate(cub3, 2.f), 3.f);
  GGO_CHECK_FLOAT_EQ(derivate(cub3, 1.f), 0.5f);
  GGO_CHECK_FLOAT_EQ(derivate(cub3, 2.f), 0.f);
}
