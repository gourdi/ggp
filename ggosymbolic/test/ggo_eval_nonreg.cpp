#include <kernel/nonreg/ggo_nonreg.h>
#include <symbolic/expressions/ggo_binary_operation.h>
#include <symbolic/expressions/ggo_constant.h>
#include <symbolic/expressions/ggo_variable.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, constants)
{
  auto add = 2._symb + 3._symb;
  GGO_CHECK_EQ(add->eval({}), 5.);

  auto sub = 5._symb - 3._symb;
  GGO_CHECK_EQ(sub->eval({}), 2.);

  auto mul = 2._symb * 3._symb;
  GGO_CHECK_EQ(mul->eval({}), 6.);

  auto div = 6._symb / 2._symb;
  GGO_CHECK_EQ(div->eval({}), 3.);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, constants_subops)
{
  auto op = 6._symb / 2._symb - 2._symb * 1._symb;
  GGO_CHECK_EQ(op->eval({}), 1.);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, add_constants_variables)
{
  auto op = "x"_symb + "y"_symb;
  GGO_CHECK_EQ(op->eval({ {"x", 2.}, {"y", 3} }), 5.);
}

