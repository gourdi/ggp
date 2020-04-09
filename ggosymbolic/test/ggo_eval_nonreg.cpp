#include <kernel/nonreg/ggo_nonreg.h>
#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_constant.h>
#include <symbolic/ggo_variable.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, constants)
{
  ggo::binary_operation add(ggo::binary_operation::add, std::make_shared<ggo::constant>(2.), std::make_shared<ggo::constant>(3.));
  GGO_CHECK_EQ(add.eval({}), 5.);

  ggo::binary_operation sub(ggo::binary_operation::sub, std::make_shared<ggo::constant>(5.), std::make_shared<ggo::constant>(3.));
  GGO_CHECK_EQ(sub.eval({}), 2.);

  ggo::binary_operation mul(ggo::binary_operation::mul, std::make_shared<ggo::constant>(2.), std::make_shared<ggo::constant>(3.));
  GGO_CHECK_EQ(mul.eval({}), 6.);

  ggo::binary_operation div(ggo::binary_operation::div, std::make_shared<ggo::constant>(6.), std::make_shared<ggo::constant>(2.));
  GGO_CHECK_EQ(div.eval({}), 3.);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, constants_subops)
{
  auto subop1 = std::make_shared<ggo::binary_operation>(ggo::binary_operation::div, std::make_shared<ggo::constant>(6.), std::make_shared<ggo::constant>(2.));
  auto subop2 = std::make_shared<ggo::binary_operation>(ggo::binary_operation::mul, std::make_shared<ggo::constant>(2.), std::make_shared<ggo::constant>(1.));

  ggo::binary_operation op(ggo::binary_operation::sub, subop1, subop2);

  GGO_CHECK_EQ(op.eval({}), 1.);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(eval, add_constants_variables)
{
  ggo::binary_operation op(ggo::binary_operation::add, std::make_shared<ggo::variable>("x"), std::make_shared<ggo::variable>("y"));

  GGO_CHECK_EQ(op.eval({ {"x", 2.}, {"y", 3} }), 5.);
}

