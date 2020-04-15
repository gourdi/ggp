#include <kernel/nonreg/ggo_nonreg.h>
#include <symbolic/expressions/ggo_binary_operation.h>
#include <symbolic/expressions/ggo_log_operation.h>
#include <symbolic/expressions/ggo_exp_operation.h>
#include <symbolic/expressions/ggo_pow_operation.h>
#include <symbolic/expressions/ggo_constant.h>
#include <symbolic/expressions/ggo_variable.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(derivate, binary_ops)
{
  auto op = ("x"_symb + 2._symb) / ("y"_symb * "x"_symb);
  GGO_CHECK_EQ(op->flat_repr(), "((x+2)/(y*x))");

  GGO_CHECK_EQ(op->derivate("x")->flat_repr(), "(((y*x)-((x+2)*y))/(y*x)^2)");
  GGO_CHECK_EQ(op->derivate("y")->flat_repr(), "((0-((x+2)*x))/(y*x)^2)");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(derivate, log_pow)
{
  auto op = ggo::log(ggo::pow("x"_symb, 5.));
  GGO_CHECK_EQ(op->flat_repr(), "log(x^5)");

  GGO_CHECK_EQ(op->derivate("x")->flat_repr(), "((5*x^4)/x^5)");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(derivate, exp)
{
  auto op = ggo::exp(ggo::pow("x"_symb, 5.), 3.);
  GGO_CHECK_EQ(op->flat_repr(), "3^(x^5)");

  GGO_CHECK_EQ(op->derivate("x")->flat_repr(), "((3^(x^5)*(5*x^4))*log(3))");
}


