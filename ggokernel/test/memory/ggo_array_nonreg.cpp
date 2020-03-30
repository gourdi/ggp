#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, construction)
{
  {
    const ggo::array_8u a(7);
    GGO_CHECK(a.count() == 7);
  }

  {
    const ggo::array1<std::string> a(3);
    GGO_CHECK_EQ(a.count(), 3);
  }

  {
    const ggo::array2_f a(7, 11);
    GGO_CHECK_EQ(a.width(), 11);
    GGO_CHECK_EQ(a.height(), 7);
    GGO_CHECK_EQ(a.rows(), 7);
    GGO_CHECK_EQ(a.cols(), 11);
    GGO_CHECK_EQ(a.dim(0), 7);
    GGO_CHECK_EQ(a.dim(1), 11);
    GGO_CHECK_EQ(a.count(), 77);
  }
}


/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, construction_and_fill)
{
  {
    const ggo::array_8u a(3, 2_u8);
    GGO_CHECK_EQ(a.count(), 3);
    GGO_CHECK_EQ(a(0), 2);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 2);
  }

  {
    const ggo::array2_f a(2, 3, 1.f);
    GGO_CHECK_EQ(a.width(), 3);
    GGO_CHECK_EQ(a.height(), 2);
    GGO_CHECK_EQ(a.rows(), 2);
    GGO_CHECK_EQ(a.cols(), 3);
    GGO_CHECK_EQ(a.dim(0), 2);
    GGO_CHECK_EQ(a.dim(1), 3);
    GGO_CHECK_EQ(a.count(), 6);
    GGO_CHECK_FLOAT_EQ(a(0, 0), 1.f);
    GGO_CHECK_FLOAT_EQ(a(0, 1), 1.f);
    GGO_CHECK_FLOAT_EQ(a(0, 2), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 0), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 1), 1.f);
    GGO_CHECK_FLOAT_EQ(a(1, 2), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, construction_from_c_array)
{
  {
    const ggo::array_i a({ 1, 2, 3, 4 });
    GGO_CHECK_EQ(a.count(), 4);
    GGO_CHECK_EQ(a(0), 1);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 3);
    GGO_CHECK_EQ(a(3), 4);
    GGO_CHECK_EQ(a.data()[0], 1);
    GGO_CHECK_EQ(a.data()[1], 2);
    GGO_CHECK_EQ(a.data()[2], 3);
    GGO_CHECK_EQ(a.data()[3], 4);
  }

  {
    const ggo::array_8u a({ 1, 2, 3, 4 });
    GGO_CHECK_EQ(a.count(), 4);
    GGO_CHECK_EQ(a(0), 1);
    GGO_CHECK_EQ(a(1), 2);
    GGO_CHECK_EQ(a(2), 3);
    GGO_CHECK_EQ(a(3), 4);
    GGO_CHECK_EQ(a.data()[0], 1);
    GGO_CHECK_EQ(a.data()[1], 2);
    GGO_CHECK_EQ(a.data()[2], 3);
    GGO_CHECK_EQ(a.data()[3], 4);
  }

  {
    const ggo::array2_i a({
      {1, 2, 3},
      {4, 5, 6} });
    GGO_CHECK_EQ(a.count(), 6);
    GGO_CHECK_EQ(a.width(), 3);
    GGO_CHECK_EQ(a.height(), 2);
    GGO_CHECK_EQ(a.rows(), 2);
    GGO_CHECK_EQ(a.cols(), 3);
    GGO_CHECK_EQ(a.dim(0), 2);
    GGO_CHECK_EQ(a.dim(1), 3);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(0, 1), 2);
    GGO_CHECK_EQ(a(0, 2), 3);
    GGO_CHECK_EQ(a(1, 0), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(1, 2), 6);
    GGO_CHECK_EQ(a.data()[0], 1);
    GGO_CHECK_EQ(a.data()[1], 2);
    GGO_CHECK_EQ(a.data()[2], 3);
    GGO_CHECK_EQ(a.data()[3], 4);
    GGO_CHECK_EQ(a.data()[4], 5);
    GGO_CHECK_EQ(a.data()[5], 6);
  }

  {
    const ggo::array2_8u a({
      { 1_u8, 2_u8, 3_u8 },
      { 4_u8, 5_u8, 6_u8 } });
    GGO_CHECK_EQ(a.count(), 6);
    GGO_CHECK_EQ(a.width(), 3);
    GGO_CHECK_EQ(a.height(), 2);
    GGO_CHECK_EQ(a.rows(), 2);
    GGO_CHECK_EQ(a.cols(), 3);
    GGO_CHECK_EQ(a.dim(0), 2);
    GGO_CHECK_EQ(a.dim(1), 3);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(0, 1), 2);
    GGO_CHECK_EQ(a(0, 2), 3);
    GGO_CHECK_EQ(a(1, 0), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(1, 2), 6);
    GGO_CHECK_EQ(a.data()[0], 1);
    GGO_CHECK_EQ(a.data()[1], 2);
    GGO_CHECK_EQ(a.data()[2], 3);
    GGO_CHECK_EQ(a.data()[3], 4);
    GGO_CHECK_EQ(a.data()[4], 5);
    GGO_CHECK_EQ(a.data()[5], 6); 
  }

  {
    const ggo::array2_8u a({
      { 1, 2, 3 },
      { 4, 5, 6 } });
    GGO_CHECK_EQ(a.count(), 6);
    GGO_CHECK_EQ(a.width(), 3);
    GGO_CHECK_EQ(a.height(), 2);
    GGO_CHECK_EQ(a.rows(), 2);
    GGO_CHECK_EQ(a.cols(), 3);
    GGO_CHECK_EQ(a.dim(0), 2);
    GGO_CHECK_EQ(a.dim(1), 3);
    GGO_CHECK_EQ(a(0, 0), 1);
    GGO_CHECK_EQ(a(0, 1), 2);
    GGO_CHECK_EQ(a(0, 2), 3);
    GGO_CHECK_EQ(a(1, 0), 4);
    GGO_CHECK_EQ(a(1, 1), 5);
    GGO_CHECK_EQ(a(1, 2), 6);
    GGO_CHECK_EQ(a.data()[0], 1);
    GGO_CHECK_EQ(a.data()[1], 2);
    GGO_CHECK_EQ(a.data()[2], 3);
    GGO_CHECK_EQ(a.data()[3], 4);
    GGO_CHECK_EQ(a.data()[4], 5);
    GGO_CHECK_EQ(a.data()[5], 6); }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, at_loop)
{
  {
    ggo::array_8u a({ 1, 2, 3 });
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-4), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-3), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-2), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 3), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 4), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 5), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 6), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>( 7), 2);
  }

  {
    ggo::array<uint16_t, 2> a({
      {1, 2, 3},
      {4, 5, 6} });
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1, -1), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1,  0), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1,  1), 5);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1,  2), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(-1,  3), 4);

    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(0, -1), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(0,  0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(0,  1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(0,  2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(0,  3), 1);

    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(1, -1), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(1,  0), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(1,  1), 5);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(1,  2), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(1,  3), 4);

    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(2, -1), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(2,  0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(2,  1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(2,  2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::loop>(2,  3), 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(ggo_array, at_mirror)
{
  {
    ggo::array_8u a({ 1, 2, 3 });
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-4), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-3), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-2), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(3), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(4), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(5), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(6), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(7), 2);
  }

  {
    ggo::array<uint16_t, 2> a({
      {1, 2, 3},
      {4, 5, 6} });
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1, -1), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1,  0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1,  1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1,  2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(-1,  3), 3);

    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0, -1), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0,  0), 1);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0,  1), 2);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0,  2), 3);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(0,  3), 3);

    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1, -1), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1,  0), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1,  1), 5);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1,  2), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(1,  3), 6);

    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2, -1), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2,  0), 4);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2,  1), 5);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2,  3), 6);
    GGO_CHECK_EQ(a.at<ggo::border_mode::mirror>(2,  2), 6);
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
  ggo::array2_8u a(2, 3, 1);
  ggo::array2_8u b(a);
  GGO_CHECK_EQ(b.height(), 2);
  GGO_CHECK_EQ(b.width(), 3);
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
