#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/linear_algebra/ggo_array_arithmetics.h>
#include <kernel/math/linear_algebra/ggo_gaussian_elimination.h>
#include <symbolic/expressions/ggo_binary_operation.h>
#include <symbolic/expressions/ggo_constant.h>
#include <symbolic/expressions/ggo_variable.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra, matrix_add)
{
  ggo::array<std::shared_ptr<const ggo::expression>, 2> m({
    { "m11"_symb, "m12"_symb },
    { "m21"_symb, "m22"_symb } });
  ggo::array<std::shared_ptr<const ggo::expression>, 2> n({
    { "n11"_symb, "n12"_symb },
    { "n21"_symb, "n22"_symb } });

  auto a = m + n;

  GGO_CHECK_EQ(a(0, 0)->flat_repr(), "(m11+n11)");
  GGO_CHECK_EQ(a(0, 1)->flat_repr(), "(m12+n12)");
  GGO_CHECK_EQ(a(1, 0)->flat_repr(), "(m21+n21)");
  GGO_CHECK_EQ(a(1, 1)->flat_repr(), "(m22+n22)");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra, matrix_mul)
{
  ggo::array<std::shared_ptr<const ggo::expression>, 2> m({
    { "m11"_symb, "m12"_symb },
    { "m21"_symb, "m22"_symb } });
  ggo::array<std::shared_ptr<const ggo::expression>, 2> n({
    { "n11"_symb, "n12"_symb },
    { "n21"_symb, "n22"_symb } });

  auto a = m * n;

  GGO_CHECK_EQ(a(0, 0)->flat_repr(), "((m11*n11)+(m12*n21))");
  GGO_CHECK_EQ(a(0, 1)->flat_repr(), "((m11*n12)+(m12*n22))");
  GGO_CHECK_EQ(a(1, 0)->flat_repr(), "((m21*n11)+(m22*n21))");
  GGO_CHECK_EQ(a(1, 1)->flat_repr(), "((m21*n12)+(m22*n22))");
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra, gaussian_elimination_2x2)
{
  ggo::array<std::shared_ptr<const ggo::expression>, 2> m({
    { "m11"_symb, "m12"_symb },
    { "m21"_symb, "m22"_symb } });
  ggo::array<std::shared_ptr<const ggo::expression>, 1> b({
    "b1"_symb,
    "b2"_symb });

  auto s = ggo::gaussian_elimination(m, b, 0._symb, [](std::shared_ptr<const ggo::expression> e) { return e->is_constant(0.); });
  std::cout << s(0)->flat_repr() << '\n' << s(1)->flat_repr() << '\n';

  const std::map<std::string, double> env({
    { "m11", 1. },
    { "m12", 1. },
    { "m21", 1. },
    { "m22", -1. },
    { "b1", 2. },
    { "b2", 1. } });
  std::cout << s(0)->eval(env) << '\n' << s(1)->eval(env) << '\n';
}



