#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_polynom.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(polynom, evaluate)
{
  const ggo::polynom<float, 0> p0({ 3.f }); // y = 3
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p0, 1.f), 3.f);

  const ggo::polynom<float, 1> p1({ 3.f, 2.f }); // y = 2*x+3
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p1, 0.f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p1, 1.f), 5.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p1, 2.f), 7.f);

  const ggo::polynom<float, 2> p2({ 3.f, 2.f, -1.f }); // y = -x^2+2*x+3
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p2, 0.f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p2, 1.f), 4.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p2, 2.f), 3.f);

  const ggo::polynom<float, 3> p3({ 3.f, 2.f, -1.f, 1.f }); // y = x^3-x^2+2*x+3
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p3, 0.f), 3.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p3, 1.f), 5.f);
  GGO_CHECK_FLOAT_EQ(ggo::evaluate(p3, 2.f), 11.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polynom, derivate_polynom)
{
  const ggo::polynom<float, 1> p1({ 3.f, 2.f }); // y = 2*x+3
  auto d1 = ggo::derivate(p1); // expecting y = 2
  GGO_CHECK_FLOAT_EQ(d1._coefs[0], 2.f);

  const ggo::polynom<float, 4> p4({ 1.f, 5.f, 9.f, 3.f, 7.f }); // y = 7*x^4+3*x^3+9*x^2+5*x+1
  auto d4 = ggo::derivate(p4); // expecting y = 28*x^3+9*x^2+18*x+5
  GGO_CHECK_FLOAT_EQ(d4._coefs[0], 5.f);
  GGO_CHECK_FLOAT_EQ(d4._coefs[1], 18.f);
  GGO_CHECK_FLOAT_EQ(d4._coefs[2], 9.f);
  GGO_CHECK_FLOAT_EQ(d4._coefs[3], 28.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polynom, derivate_value)
{
  const ggo::polynom<float, 1> p1({ 3.f, 2.f }); // y = 2*x+3
  GGO_CHECK_FLOAT_EQ(ggo::derivate(p1, 5.f), 2.f);

  const ggo::polynom<float, 4> p4({ 1.f, 5.f, 9.f, 3.f, 7.f }); // y = 7*x^4+3*x^3+9*x^2+5*x+1
  GGO_CHECK_FLOAT_EQ(ggo::derivate(p4, 2.f), 301.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polynom, integrate_polynom)
{
  const ggo::polynom<float, 0> p0({ 3.f }); // y = 3
  auto i0 = ggo::integrate(p0, 1.f); // expecting y = 3*x+1
  GGO_CHECK_FLOAT_EQ(i0._coefs[0], 1.f);
  GGO_CHECK_FLOAT_EQ(i0._coefs[1], 3.f);

  const ggo::polynom<float, 1> p1({ 3.f, 2.f }); // y = 2*x+3
  auto i1 = ggo::integrate(p1, 1.f); // expecting y = x^2+3*x+1
  GGO_CHECK_FLOAT_EQ(i1._coefs[0], 1.f);
  GGO_CHECK_FLOAT_EQ(i1._coefs[1], 3.f);
  GGO_CHECK_FLOAT_EQ(i1._coefs[2], 1.f);

  const ggo::polynom<float, 2> p2({ 3.f, 2.f, 1.f }); // y = x^2+2*x+3
  auto i2 = ggo::integrate(p2, 1.f); // expecting y = x^3/3+x^2+3*x+1
  GGO_CHECK_FLOAT_EQ(i2._coefs[0], 1.f);
  GGO_CHECK_FLOAT_EQ(i2._coefs[1], 3.f);
  GGO_CHECK_FLOAT_EQ(i2._coefs[2], 1.f);
  GGO_CHECK_FLOAT_EQ(i2._coefs[3], 1.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polynom, integrate_value)
{
  const ggo::polynom<float, 0> p0({ 3.f }); // integrate y = 3 from 1 to 3
  GGO_CHECK_FLOAT_EQ(ggo::integrate(p0, 1.f, 3.f), 6.f);

  const ggo::polynom<float, 1> p1({ 3.f, 2.f }); // integrate y = 2*x+3 from 1 to 3
  GGO_CHECK_FLOAT_EQ(ggo::integrate(p1, 1.f, 3.f), 14.f);

  const ggo::polynom<float, 3> p3({ 7.f, 4.f, 9.f, 20.f }); // y = 20*x^3+9*x^2+4*x+è
  GGO_CHECK_FLOAT_EQ(ggo::integrate(p3, -1.f, 1.f), 20.f);
}
