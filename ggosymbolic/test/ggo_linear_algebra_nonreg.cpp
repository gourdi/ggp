#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/linear_algebra/ggo_array_arithmetics.h>
#include <kernel/math/linear_algebra/ggo_gaussian_elimination.h>
#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_constant.h>
#include <symbolic/ggo_variable.h>

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
GGO_TEST(linear_algebra, gaussian_elimination_2x2_constants)
{
  ggo::array<std::shared_ptr<const ggo::expression>, 2> m({
    { 3._symb, 1._symb },
    { 1._symb, 1._symb } });
  ggo::array<std::shared_ptr<const ggo::expression>, 1> b({
    1._symb,
    2._symb });

  auto s = ggo::gaussian_elimination(m, b, 0._symb, [](std::shared_ptr<const ggo::expression> e) { auto cst = e->get_constant(); return cst && *cst == 0.; });
  GGO_CHECK(s(0)->get_constant().has_value());
  GGO_CHECK(s(1)->get_constant().has_value());
  GGO_CHECK_EQ(*s(0)->get_constant(), -0.5);
  GGO_CHECK_EQ(*s(1)->get_constant(), 2.5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(linear_algebra, gaussian_elimination_2x2_generic)
{
  ggo::array<std::shared_ptr<const ggo::expression>, 2> m({
    { "m11"_symb, "m12"_symb },
    { "m21"_symb, "m22"_symb } });
  ggo::array<std::shared_ptr<const ggo::expression>, 1> b({
    "b1"_symb,
    "b2"_symb });

  auto s = ggo::gaussian_elimination(m, b, 0._symb, [](std::shared_ptr<const ggo::expression> e) { auto cst = e->get_constant(); return cst && *cst == 0.; });

  // Check solutions really are solutions by plugging them into the system using some real value as parameters.
  const double b1 = 2;
  const double b2 = 1;
  const std::map<std::string, double> env({
    { "m11", 1. }, { "m12", 1. },
    { "m21", 1. }, { "m22", -1. },
    { "b1", b1 },
    { "b2", b2 } });
  auto eq1 = "m11"_symb * s(0) + "m12"_symb * s(1);
  auto eq2 = "m21"_symb * s(0) + "m22"_symb * s(1);
  GGO_CHECK_EQ(eq1->eval(env), b1);
  GGO_CHECK_EQ(eq2->eval(env), b2);
}



