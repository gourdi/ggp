#include <ggo_nonreg.h>
#include <ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, construction)
{  
  {
    ggo::array<uint8_t, 1> a(7);
    GGO_CHECK(a.get_size<0>() == 7);
    GGO_CHECK(a.get_count() == 7);
  }

  {
    ggo::array<std::string, 1> a(3);
    GGO_CHECK_EQ(a.get_size<0>(), 3);
    GGO_CHECK_EQ(a.get_count(),3);
  }

  {
    ggo::array<float, 2> a(7, 11);
    GGO_CHECK_EQ(a.get_size<0>(), 7);
    GGO_CHECK_EQ(a.get_size<1>(), 11);
    GGO_CHECK_EQ(a.get_count(), 77);
  }

  {
    ggo::array<int, 1> a({ 1, 2, 3, 4 });
    GGO_CHECK_EQ(a.get_size<0>(), 4);
    GGO_CHECK_EQ(a(0), 1);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 3);
    GGO_CHECK_EQ(a(3), 4);
  }

  {
    ggo::array<uint8_t, 1> a({ 1_u8, 2_u8, 3_u8, 4_u8 });
    GGO_CHECK_EQ(a.get_size<0>(), 4);
    GGO_CHECK_EQ(a(0), 1);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 3);
    GGO_CHECK_EQ(a(3), 4);
  }

  {
    ggo::array<uint8_t, 1> a({ 1, 2, 3, 4 });
    GGO_CHECK_EQ(a.get_size<0>(), 4);
    GGO_CHECK_EQ(a(0), 1);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 3);
    GGO_CHECK_EQ(a(3), 4);
  }

  {
    ggo::array<int, 2> a({
      {1, 2, 3},
      {4, 5, 6} });
    GGO_CHECK_EQ(a.get_count(), 6);
    GGO_CHECK_EQ(a.get_size<0>(), 3);
    GGO_CHECK_EQ(a.get_size<1>(), 2);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(1, 0), 2);
    GGO_CHECK_EQ(a(2, 0), 3);
    GGO_CHECK_EQ(a(0, 1), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(2, 1), 6);
  }

  {
    ggo::array<uint8_t, 2> a({
      { 1_u8, 2_u8, 3_u8 },
      { 4_u8, 5_u8, 6_u8 } });
    GGO_CHECK_EQ(a.get_count(), 6);
    GGO_CHECK_EQ(a.get_size<0>(), 3);
    GGO_CHECK_EQ(a.get_size<1>(), 2);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(1, 0), 2);
    GGO_CHECK_EQ(a(2, 0), 3);
    GGO_CHECK_EQ(a(0, 1), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(2, 1), 6);
  }

  {
    ggo::array<uint8_t, 2> a({
      { 1, 2, 3 },
      { 4, 5, 6 } });
    GGO_CHECK_EQ(a.get_count(), 6);
    GGO_CHECK_EQ(a.get_size<0>(), 3);
    GGO_CHECK_EQ(a.get_size<1>(), 2);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(1, 0), 2);
    GGO_CHECK_EQ(a(2, 0), 3);
    GGO_CHECK_EQ(a(0, 1), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(2, 1), 6);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, construction_and_fill)
{
  {
    ggo::array<uint8_t, 1> a(3, 2);
    GGO_CHECK_EQ(a.get_size<0>(), 3);
    GGO_CHECK_EQ(a.get_count(), 3);
    GGO_CHECK_EQ(a(0), 2);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 2);
  }

  {
    ggo::array<float, 2> a(2, 3, 1.f);
    GGO_CHECK_EQ(a.get_size<0>(), 2);
    GGO_CHECK_EQ(a.get_size<1>(), 3);
    GGO_CHECK_EQ(a.get_count(), 6);
    GGO_CHECK_FLOAT_EQ(a(0, 0), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 0), 1.f);
    GGO_CHECK_FLOAT_EQ(a(0, 1), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 1), 1.f);
    GGO_CHECK_FLOAT_EQ(a(0, 2), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 2), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, resize)
{
  ggo::array<uint8_t, 2> a(2, 3);
  a.resize(10, 5);
  GGO_CHECK_EQ(a.get_size<0>(), 10);
  GGO_CHECK_EQ(a.get_size<1>(), 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, access)
{
  {
    ggo::array<int, 1> a(7);
    a(0) = 2;
    a(1) = 4;
    a(2) = 7;
    a(3) = 1;
    a(4) = 0;
    a(5) = 8;
    a(6) = 9;
    GGO_CHECK(a(0) == 2);
    GGO_CHECK(a(1) == 4);
    GGO_CHECK(a(2) == 7);
    GGO_CHECK(a(3) == 1);
    GGO_CHECK(a(4) == 0);
    GGO_CHECK(a(5) == 8);
    GGO_CHECK(a(6) == 9);
  }

  {
    ggo::array<int, 2> a(2, 3);
    a(0, 0) = 2;
    a(1, 0) = 5;
    a(0, 1) = 7;
    a(1, 1) = 9;
    a(0, 2) = 0;
    a(1, 2) = 1;
    GGO_CHECK(a(0, 0) == 2);
    GGO_CHECK(a(1, 0) == 5);
    GGO_CHECK(a(0, 1) == 7);
    GGO_CHECK(a(1, 1) == 9);
    GGO_CHECK(a(0, 2) == 0);
    GGO_CHECK(a(1, 2) == 1);

    const int * data = a.data();
    GGO_CHECK(data[0] == 2);
    GGO_CHECK(data[1] == 5);
    GGO_CHECK(data[2] == 7);
    GGO_CHECK(data[3] == 9);
    GGO_CHECK(data[4] == 0);
    GGO_CHECK(data[5] == 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, loop_access)
{
  {
    ggo::array_uint8 a(3);
    a(0) = 1;
    a(1) = 2;
    a(2) = 3;
    GGO_CHECK_EQ(a.get_loop(-4), 3);
    GGO_CHECK_EQ(a.get_loop(-3), 1);
    GGO_CHECK_EQ(a.get_loop(-2), 2);
    GGO_CHECK_EQ(a.get_loop(-1), 3);
    GGO_CHECK_EQ(a.get_loop( 0), 1);
    GGO_CHECK_EQ(a.get_loop( 1), 2);
    GGO_CHECK_EQ(a.get_loop( 2), 3);
    GGO_CHECK_EQ(a.get_loop( 3), 1);
    GGO_CHECK_EQ(a.get_loop( 4), 2);
    GGO_CHECK_EQ(a.get_loop( 5), 3);
    GGO_CHECK_EQ(a.get_loop( 6), 1);
    GGO_CHECK_EQ(a.get_loop( 7), 2);
  }

  {
    ggo::array<uint16_t, 2> a(3, 2);
    a(0, 0) = 1;
    a(1, 0) = 2;
    a(2, 0) = 3;
    a(0, 1) = 4;
    a(1, 1) = 5;
    a(2, 1) = 6;
    GGO_CHECK_EQ(a.get_loop(-1, -1), 6);
    GGO_CHECK_EQ(a.get_loop( 0, -1), 4);
    GGO_CHECK_EQ(a.get_loop( 1, -1), 5);
    GGO_CHECK_EQ(a.get_loop( 2, -1), 6);
    GGO_CHECK_EQ(a.get_loop( 3, -1), 4);

    GGO_CHECK_EQ(a.get_loop(-1, 0), 3);
    GGO_CHECK_EQ(a.get_loop( 0, 0), 1);
    GGO_CHECK_EQ(a.get_loop( 1, 0), 2);
    GGO_CHECK_EQ(a.get_loop( 2, 0), 3);
    GGO_CHECK_EQ(a.get_loop( 3, 0), 1);

    GGO_CHECK_EQ(a.get_loop(-1, 1), 6);
    GGO_CHECK_EQ(a.get_loop( 0, 1), 4);
    GGO_CHECK_EQ(a.get_loop( 1, 1), 5);
    GGO_CHECK_EQ(a.get_loop( 2, 1), 6);
    GGO_CHECK_EQ(a.get_loop( 3, 1), 4);

    GGO_CHECK_EQ(a.get_loop(-1, 2), 3);
    GGO_CHECK_EQ(a.get_loop( 0, 2), 1);
    GGO_CHECK_EQ(a.get_loop( 1, 2), 2);
    GGO_CHECK_EQ(a.get_loop( 2, 2), 3);
    GGO_CHECK_EQ(a.get_loop( 3, 2), 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, loop_mirror)
{
  {
    ggo::array_uint8 a(3);
    a(0) = 1;
    a(1) = 2;
    a(2) = 3;
    GGO_CHECK_EQ(a.get_mirror(-4), 3);
    GGO_CHECK_EQ(a.get_mirror(-3), 3);
    GGO_CHECK_EQ(a.get_mirror(-2), 2);
    GGO_CHECK_EQ(a.get_mirror(-1), 1);
    GGO_CHECK_EQ(a.get_mirror(0), 1);
    GGO_CHECK_EQ(a.get_mirror(1), 2);
    GGO_CHECK_EQ(a.get_mirror(2), 3);
    GGO_CHECK_EQ(a.get_mirror(3), 3);
    GGO_CHECK_EQ(a.get_mirror(4), 2);
    GGO_CHECK_EQ(a.get_mirror(5), 1);
    GGO_CHECK_EQ(a.get_mirror(6), 1);
    GGO_CHECK_EQ(a.get_mirror(7), 2);
  }

  {
    ggo::array<uint16_t, 2> a(3, 2);
    a(0, 0) = 1;
    a(1, 0) = 2;
    a(2, 0) = 3;
    a(0, 1) = 4;
    a(1, 1) = 5;
    a(2, 1) = 6;
    GGO_CHECK_EQ(a.get_mirror(-1, -1), 1);
    GGO_CHECK_EQ(a.get_mirror(0, -1), 1);
    GGO_CHECK_EQ(a.get_mirror(1, -1), 2);
    GGO_CHECK_EQ(a.get_mirror(2, -1), 3);
    GGO_CHECK_EQ(a.get_mirror(3, -1), 3);

    GGO_CHECK_EQ(a.get_mirror(-1, 0), 1);
    GGO_CHECK_EQ(a.get_mirror(0, 0), 1);
    GGO_CHECK_EQ(a.get_mirror(1, 0), 2);
    GGO_CHECK_EQ(a.get_mirror(2, 0), 3);
    GGO_CHECK_EQ(a.get_mirror(3, 0), 3);

    GGO_CHECK_EQ(a.get_mirror(-1, 1), 4);
    GGO_CHECK_EQ(a.get_mirror(0, 1), 4);
    GGO_CHECK_EQ(a.get_mirror(1, 1), 5);
    GGO_CHECK_EQ(a.get_mirror(2, 1), 6);
    GGO_CHECK_EQ(a.get_mirror(3, 1), 6);

    GGO_CHECK_EQ(a.get_mirror(-1, 2), 4);
    GGO_CHECK_EQ(a.get_mirror(0, 2), 4);
    GGO_CHECK_EQ(a.get_mirror(1, 2), 5);
    GGO_CHECK_EQ(a.get_mirror(2, 2), 6);
    GGO_CHECK_EQ(a.get_mirror(3, 2), 6);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, fill)
{
  {
    ggo::array<float, 1> a(5);
    a.fill(8);
    GGO_CHECK(a(0) == 8);
    GGO_CHECK(a(1) == 8);
    GGO_CHECK(a(2) == 8);
    GGO_CHECK(a(3) == 8);
    GGO_CHECK(a(4) == 8);
  }

  {
    ggo::array<float, 2> a(3, 2);
    a.fill(8);
    GGO_CHECK(a(0, 0) == 8);
    GGO_CHECK(a(1, 0) == 8);
    GGO_CHECK(a(2, 0) == 8);
    GGO_CHECK(a(0, 1) == 8);
    GGO_CHECK(a(1, 1) == 8);
    GGO_CHECK(a(2, 1) == 8);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, range_loop)
{
  auto test_range_loop_aux = [&](const ggo::array<int, 1> & array, int ref_sum, int ref_count)
  {
    int sum = 0;
    int count = 0;
    for (const auto & v : array)
    {
      sum += v;
      count += 1;
    }

    GGO_CHECK(sum == ref_sum);
    GGO_CHECK(count == ref_count);
  };

  ggo::array<int, 1> array(3);
  array(0) = 2;
  array(1) = 5;
  array(2) = 7;

  test_range_loop_aux(array, 14, 3);

  for (auto & v : array)
  {
    v += 2;
  }

  test_range_loop_aux(array, 20, 3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, copy)
{
  ggo::array<uint8_t, 2> a(2, 3, 1);
  ggo::array<uint8_t, 2> b(a);
  GGO_CHECK_EQ(b.get_size<0>(), 2);
  GGO_CHECK_EQ(b.get_size<1>(), 3);
  for (auto v : b)
  {
    GGO_CHECK_EQ(v, 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, move)
{
  auto lambda = [](int width, int height, int fill_value) { return ggo::array<int, 2>(width, height, fill_value); };

  ggo::array<int, 2> a(lambda(2, 1, 1));
  GGO_CHECK_EQ(a(0, 0), 1);
  GGO_CHECK_EQ(a(1, 0), 1);

  a = lambda(3, 2, 2);
  GGO_CHECK_EQ(a(0, 0), 2);
  GGO_CHECK_EQ(a(1, 0), 2);
  GGO_CHECK_EQ(a(2, 0), 2);
  GGO_CHECK_EQ(a(0, 1), 2);
  GGO_CHECK_EQ(a(1, 1), 2);
  GGO_CHECK_EQ(a(2, 1), 2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, comparison)
{
  ggo::array<int, 2> a(2, 3);
  a(0, 0) = 2;
  a(1, 0) = 5;
  a(0, 1) = 7;
  a(1, 1) = 9;
  a(0, 2) = 0;
  a(1, 2) = 1;

  ggo::array<int, 2> b(2, 3);
  b(0, 0) = 2;
  b(1, 0) = 5;
  b(0, 1) = 7;
  b(1, 1) = 9;
  b(0, 2) = 0;
  b(1, 2) = 1;
  GGO_CHECK_EQ(a, b);

  b(1, 2) = 2;
  GGO_CHECK_NE(a, b);

  b = ggo::array<int, 2>(2, 2, 0);
  GGO_CHECK_NE(a, b);
}
