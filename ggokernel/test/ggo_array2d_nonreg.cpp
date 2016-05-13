#include <ggo_nonreg.h>
#include <ggo_array2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(array2d, swap)
{
  ggo::array2d<int> a(2, 2);
  a(0, 0) = 1;
  a(0, 1) = 2;
  a(1, 0) = 3;
  a(1, 1) = 4;

  ggo::array2d<int> b(3, 1);
  b(0, 0) = 5;
  b(1, 0) = 6;
  b(2, 0) = 7;

  std::swap(a, b);

  GGO_CHECK(a.get_size_x() == 3);
  GGO_CHECK(a.get_size_y() == 1);
  GGO_CHECK(a(0, 0) == 5);
  GGO_CHECK(a(1, 0) == 6);
  GGO_CHECK(a(2, 0) == 7);

  GGO_CHECK(b.get_size_x() == 2);
  GGO_CHECK(b.get_size_y() == 2);
  GGO_CHECK(b(0, 0) == 1);
  GGO_CHECK(b(0, 1) == 2);
  GGO_CHECK(b(1, 0) == 3);
  GGO_CHECK(b(1, 1) == 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(array2d, copy)
{
  ggo::array2d<int> a(2, 2);
  a(0, 0) = 1;
  a(0, 1) = 2;
  a(1, 0) = 3;
  a(1, 1) = 4;

  ggo::array2d<int> b(a);
  GGO_CHECK(b.get_size_x() == 2);
  GGO_CHECK(b.get_size_y() == 2);
  GGO_CHECK(b(0, 0) == 1);
  GGO_CHECK(b(0, 1) == 2);
  GGO_CHECK(b(1, 0) == 3);
  GGO_CHECK(b(1, 1) == 4);

  ggo::array2d<int> c(1, 1, 0);
  GGO_CHECK(c.get_size_x() == 1);
  GGO_CHECK(c.get_size_y() == 1);
  GGO_CHECK(c(0, 0) == 0);
  c = a;
  GGO_CHECK(c.get_size_x() == 2);
  GGO_CHECK(c.get_size_y() == 2);
  GGO_CHECK(c(0, 0) == 1);
  GGO_CHECK(c(0, 1) == 2);
  GGO_CHECK(c(1, 0) == 3);
  GGO_CHECK(c(1, 1) == 4);
}
