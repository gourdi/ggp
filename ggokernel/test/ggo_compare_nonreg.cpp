#include <kernel/ggo_compare.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(compare, fixed_size_array)
{
  constexpr int b1[3] = { 1, 3, 8 };
  constexpr int b2[3] = { 1, 3, 8 };
  constexpr int b3[3] = { 1, 3, 9 };

  static_assert(ggo::compare(b1, b2) == true);
  static_assert(ggo::compare(b1, b3) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(compare, pointers)
{
  int * ptr1 = new int[3]; ptr1[0] = 1; ptr1[1] = 3; ptr1[2] = 8;
  int * ptr2 = new int[3]; ptr2[0] = 1; ptr2[1] = 3; ptr2[2] = 8;
  int * ptr3 = new int[3]; ptr3[0] = 1; ptr3[1] = 3; ptr3[2] = 9;

  GGO_CHECK(ggo::compare<3>(ptr1, ptr2) == true);
  GGO_CHECK(ggo::compare<3>(ptr1, ptr3) == false);

  delete[] ptr1;
  delete[] ptr2;
  delete[] ptr3;
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(compare, fixed_size_array_float)
{
  const float b1[3] = { 1, 3, 8 };
  const float b2[3] = { 1, 3, 8.001f };

  GGO_CHECK(ggo::compare(b1, b2, 0.0001f) == false);
  GGO_CHECK(ggo::compare(b1, b2, 0.01f) == true);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(compare, pointers_float)
{
  float * ptr1 = new float[3]; ptr1[0] = 1; ptr1[1] = 3; ptr1[2] = 8;
  float * ptr2 = new float[3]; ptr2[0] = 1; ptr2[1] = 3; ptr2[2] = 8.001f;

  GGO_CHECK(ggo::compare<3>(ptr1, ptr2, 0.0001f) == false);
  GGO_CHECK(ggo::compare<3>(ptr1, ptr2, 0.01f) == true);

  delete[] ptr1;
  delete[] ptr2;
}

