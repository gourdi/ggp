#include <kernel/ggo_assign.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(assign, fixed_size_array)
{
  int b[3];

  ggo::assign(b, 1, 3, 8);

  GGO_CHECK_EQ(b[0], 1);
  GGO_CHECK_EQ(b[1], 3);
  GGO_CHECK_EQ(b[2], 8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(assign, fixed_size_array_copy)
{
  int dst[3];
  const int src[3] = { 1, 3, 8 };

  ggo::assign(dst, src);

  GGO_CHECK_EQ(dst[0], 1);
  GGO_CHECK_EQ(dst[1], 3);
  GGO_CHECK_EQ(dst[2], 8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(assign, buffer_copy)
{
  int * dst = new int[3];
  int * src = new int[3]; src[0] = 1; src[1] = 3; src[2] = 8;

  ggo::assign<3>(dst, src);

  GGO_CHECK_EQ(dst[0], 1);
  GGO_CHECK_EQ(dst[1], 3);
  GGO_CHECK_EQ(dst[2], 8);

  delete[] dst;
  delete[] src;
}

