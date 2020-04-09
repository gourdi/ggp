#include <kernel/nonreg/ggo_nonreg.h>
#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_constant.h>
#include <symbolic/ggo_variable.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(derivate, constants)
{
  auto subop1 = ggo::make_add("x"_symb, 2._symb);
  auto subop2 = ggo::make_mul("y"_symb, "x"_symb);

  auto op = ggo::make_div(subop1, subop2);
  std::cout << op->flat_repr() << "\n";

  auto dx = op->derivate("x");
  std::cout << dx->flat_repr() << "\n";

  auto dy = op->derivate("y");
  std::cout << dy->flat_repr() << "\n";

}
