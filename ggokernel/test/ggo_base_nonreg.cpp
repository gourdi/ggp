#include <ggo_nonreg.h>
#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, loop_index)
{
  GGO_CHECK(ggo::loop_index(-6, 4) == 2);
  GGO_CHECK(ggo::loop_index(-5, 4) == 3);
  GGO_CHECK(ggo::loop_index(-4, 4) == 0);
  GGO_CHECK(ggo::loop_index(-3, 4) == 1);
  GGO_CHECK(ggo::loop_index(-2, 4) == 2);
  GGO_CHECK(ggo::loop_index(-1, 4) == 3);
  GGO_CHECK(ggo::loop_index( 0, 4) == 0);
  GGO_CHECK(ggo::loop_index( 1, 4) == 1);
  GGO_CHECK(ggo::loop_index( 2, 4) == 2);
  GGO_CHECK(ggo::loop_index( 3, 4) == 3);
  GGO_CHECK(ggo::loop_index( 4, 4) == 0);
  GGO_CHECK(ggo::loop_index( 5, 4) == 1);
  GGO_CHECK(ggo::loop_index( 6, 4) == 2);
  GGO_CHECK(ggo::loop_index( 7, 4) == 3);
  GGO_CHECK(ggo::loop_index( 8, 4) == 0);
  GGO_CHECK(ggo::loop_index( 9, 4) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, mirror_index)
{
  // Edge duplicated.
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-6, 4) == 2);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-5, 4) == 3);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-4, 4) == 3);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-3, 4) == 2);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-2, 4) == 1);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(-1, 4) == 0);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(0, 4) == 0);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(1, 4) == 1);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(2, 4) == 2);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(3, 4) == 3);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(4, 4) == 3);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(5, 4) == 2);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(6, 4) == 1);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(7, 4) == 0);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(8, 4) == 0);
  GGO_CHECK(ggo::mirror_index_edge_duplicated(9, 4) == 1);

  // Single edge.
  GGO_CHECK(ggo::mirror_index_single_edge(-6, 5) == 2);
  GGO_CHECK(ggo::mirror_index_single_edge(-5, 5) == 3);
  GGO_CHECK(ggo::mirror_index_single_edge(-4, 5) == 4);
  GGO_CHECK(ggo::mirror_index_single_edge(-3, 5) == 3);
  GGO_CHECK(ggo::mirror_index_single_edge(-2, 5) == 2);
  GGO_CHECK(ggo::mirror_index_single_edge(-1, 5) == 1);
  GGO_CHECK(ggo::mirror_index_single_edge(0, 5) == 0);
  GGO_CHECK(ggo::mirror_index_single_edge(1, 5) == 1);
  GGO_CHECK(ggo::mirror_index_single_edge(2, 5) == 2);
  GGO_CHECK(ggo::mirror_index_single_edge(3, 5) == 3);
  GGO_CHECK(ggo::mirror_index_single_edge(4, 5) == 4);
  GGO_CHECK(ggo::mirror_index_single_edge(5, 5) == 3);
  GGO_CHECK(ggo::mirror_index_single_edge(6, 5) == 2);
  GGO_CHECK(ggo::mirror_index_single_edge(7, 5) == 1);
  GGO_CHECK(ggo::mirror_index_single_edge(8, 5) == 0);
  GGO_CHECK(ggo::mirror_index_single_edge(9, 5) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, type_casting)
{
  // From float.
  GGO_CHECK_FABS(ggo::to<float>(1.1f), 1.1f);
  GGO_CHECK_FABS(ggo::to<double>(1.1f), 1.1);
  
  GGO_CHECK(ggo::to<int8_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<int8_t>(-312.f) == -128);
  GGO_CHECK(ggo::to<int8_t>(312.f) == 127);
  
  GGO_CHECK(ggo::to<uint8_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<uint8_t>(-1.9f) == 0);
  GGO_CHECK(ggo::to<uint8_t>(312.f) == 255);

  GGO_CHECK(ggo::to<int16_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<int16_t>(-80000.f) == -32768);
  GGO_CHECK(ggo::to<int16_t>(80000.f) == 32767);
  
  GGO_CHECK(ggo::to<uint16_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<uint16_t>(-80000.f) == 0);
  GGO_CHECK(ggo::to<uint16_t>(80000.f) == 65535);
  
  GGO_CHECK(ggo::to<int32_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<int32_t>(-80000.f) == -80000);
  GGO_CHECK(ggo::to<int32_t>(80000.f) == 80000);
  
  GGO_CHECK(ggo::to<uint32_t>(1.9f) == 2);
  GGO_CHECK(ggo::to<uint32_t>(-80000.f) == 0);
  GGO_CHECK(ggo::to<uint32_t>(80000.f) == 80000);
  
  GGO_CHECK(ggo::to<int>(-0.9f) == -1);
  GGO_CHECK(ggo::to<int>(-2.1f) == -2);
  GGO_CHECK(ggo::to<int>(0.9f) == 1);
  GGO_CHECK(ggo::to<int>(2.1f) == 2);
  
  // From double.
  GGO_CHECK_FABS(ggo::to<float>(1.1), 1.1f);
  GGO_CHECK_FABS(ggo::to<double>(1.1), 1.1);
  
  GGO_CHECK(ggo::to<int8_t>(1.9) == 2);
  GGO_CHECK(ggo::to<int8_t>(-312.) == -128);
  GGO_CHECK(ggo::to<int8_t>(312.) == 127);
  
  GGO_CHECK(ggo::to<uint8_t>(1.9) == 2);
  GGO_CHECK(ggo::to<uint8_t>(-1.9) == 0);
  GGO_CHECK(ggo::to<uint8_t>(312.) == 255);
  
  GGO_CHECK(ggo::to<int16_t>(1.9) == 2);
  GGO_CHECK(ggo::to<int16_t>(-80000.) == -32768);
  GGO_CHECK(ggo::to<int16_t>(80000.) == 32767);
  
  GGO_CHECK(ggo::to<uint16_t>(1.9) == 2);
  GGO_CHECK(ggo::to<uint16_t>(-80000.) == 0);
  GGO_CHECK(ggo::to<uint16_t>(80000.) == 65535);

  GGO_CHECK(ggo::to<int32_t>(1.9) == 2);
  GGO_CHECK(ggo::to<int32_t>(-80000.) == -80000);
  GGO_CHECK(ggo::to<int32_t>(80000.) == 80000);
  
  GGO_CHECK(ggo::to<uint32_t>(1.9) == 2);
  GGO_CHECK(ggo::to<uint32_t>(-80000.) == 0);
  GGO_CHECK(ggo::to<uint32_t>(80000.) == 80000);
  
  GGO_CHECK(ggo::to<int>(-0.9) == -1);
  GGO_CHECK(ggo::to<int>(-2.1) == -2);
  GGO_CHECK(ggo::to<int>(0.9) == 1);
  GGO_CHECK(ggo::to<int>(2.1) == 2);
  
  // From int8_t
  GGO_CHECK(ggo::to<uint8_t>(int8_t(-15)) == 0);
  GGO_CHECK(ggo::to<uint8_t>(int8_t(15)) == 15);
  GGO_CHECK(ggo::to<uint16_t>(int8_t(-15)) == 0);
  GGO_CHECK(ggo::to<uint16_t>(int8_t(15)) == 15);
  GGO_CHECK(ggo::to<uint32_t>(int8_t(-15)) == 0);
  GGO_CHECK(ggo::to<uint32_t>(int8_t(15)) == 15);
  
  GGO_CHECK(ggo::to<int8_t>(int8_t(-15)) == -15);
  GGO_CHECK(ggo::to<int8_t>(int8_t(15)) == 15);
  GGO_CHECK(ggo::to<int16_t>(int8_t(-15)) == -15);
  GGO_CHECK(ggo::to<int16_t>(int8_t(15)) == 15);
  GGO_CHECK(ggo::to<int32_t>(int8_t(-15)) == -15);
  GGO_CHECK(ggo::to<int32_t>(int8_t(15)) == 15);

  // From int16_t
  GGO_CHECK(ggo::to<uint8_t>(int16_t(-15000)) == 0);
  GGO_CHECK(ggo::to<uint8_t>(int16_t(15000)) == 255);
  GGO_CHECK(ggo::to<uint16_t>(int16_t(-15000)) == 0);
  GGO_CHECK(ggo::to<uint16_t>(int16_t(15000)) == 15000);
  GGO_CHECK(ggo::to<uint32_t>(int16_t(-15000)) == 0);
  GGO_CHECK(ggo::to<uint32_t>(int16_t(15000)) == 15000);
  
  GGO_CHECK(ggo::to<int8_t>(int16_t(-15000)) == -128);
  GGO_CHECK(ggo::to<int8_t>(int16_t(15000)) == 127);
  GGO_CHECK(ggo::to<int16_t>(int16_t(-15000)) == -15000);
  GGO_CHECK(ggo::to<int16_t>(int16_t(15000)) == 15000);
  GGO_CHECK(ggo::to<int32_t>(int16_t(-15000)) == -15000);
  GGO_CHECK(ggo::to<int32_t>(int16_t(15000)) == 15000);
  
  // From int32_t
  GGO_CHECK(ggo::to<uint8_t>(int32_t(-150000)) == 0);
  GGO_CHECK(ggo::to<uint8_t>(int32_t(150000)) == 255);
  GGO_CHECK(ggo::to<uint16_t>(int32_t(-150000)) == 0);
  GGO_CHECK(ggo::to<uint16_t>(int32_t(150000)) == 65535);
  GGO_CHECK(ggo::to<uint32_t>(int32_t(-150000)) == 0);
  GGO_CHECK(ggo::to<uint32_t>(int32_t(150000)) == 150000);
  
  GGO_CHECK(ggo::to<int8_t>(int32_t(-150000)) == -128);
  GGO_CHECK(ggo::to<int8_t>(int32_t(150000)) == 127);
  GGO_CHECK(ggo::to<int16_t>(int32_t(-150000)) == -32768);
  GGO_CHECK(ggo::to<int16_t>(int32_t(150000)) == 32767);
  GGO_CHECK(ggo::to<int32_t>(int32_t(-150000)) == -150000);
  GGO_CHECK(ggo::to<int32_t>(int32_t(150000)) == 150000);
  
  // From uint8_t
  GGO_CHECK(ggo::to<uint8_t>(uint8_t(255)) == 255);
  GGO_CHECK(ggo::to<uint16_t>(uint8_t(255)) == 255);
  GGO_CHECK(ggo::to<uint32_t>(uint8_t(255)) == 255);
  
  GGO_CHECK(ggo::to<int8_t>(uint8_t(255)) == 127);
  GGO_CHECK(ggo::to<int16_t>(uint8_t(255)) == 255);
  GGO_CHECK(ggo::to<int32_t>(uint8_t(255)) == 255);

  // From uint16_t
  GGO_CHECK(ggo::to<uint8_t>(uint16_t(65535)) == 255);
  GGO_CHECK(ggo::to<uint16_t>(uint16_t(65535)) == 65535);
  GGO_CHECK(ggo::to<uint32_t>(uint16_t(65535)) == 65535);
  
  GGO_CHECK(ggo::to<int8_t>(uint16_t(65535)) == 127);
  GGO_CHECK(ggo::to<int16_t>(uint16_t(65535)) == 32767);
  GGO_CHECK(ggo::to<int32_t>(uint16_t(65535)) == 65535);
  
  // From uint32_t
  GGO_CHECK(ggo::to<uint8_t>(uint32_t(0xFFFFFFFF)) == 255);
  GGO_CHECK(ggo::to<uint16_t>(uint32_t(0xFFFFFFFF)) == 65535);
  GGO_CHECK(ggo::to<uint32_t>(uint32_t(0xFFFFFFFF)) == 0xFFFFFFFF);
  
  GGO_CHECK(ggo::to<int8_t>(uint32_t(0xFFFFFFFF)) == 127);
  GGO_CHECK(ggo::to<int16_t>(uint32_t(0xFFFFFFFF)) == 32767);
  GGO_CHECK(ggo::to<int32_t>(uint32_t(0xFFFFFFFF)) == std::numeric_limits<int32_t>::max());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, remove_first_if)
{
  {
    std::vector<int> v{ 6, 2, 3, 4, 5 };
    ggo::remove_first_if(v, [](int i) { return i % 2 == 1; }); // Remove first odd number.
    GGO_CHECK_EQ(4, v.size());
    GGO_CHECK_EQ(6, v[0]);
    GGO_CHECK_EQ(2, v[1]);
    GGO_CHECK_EQ(4, v[2]);
    GGO_CHECK_EQ(5, v[3]);
  }

  {
    std::vector<int> v{ 5, 1, 3 };
    ggo::remove_first_if(v, [](int i) { return i % 2 == 0; }); // Remove first even number.
    GGO_CHECK_EQ(3, v.size());
    GGO_CHECK_EQ(5, v[0]);
    GGO_CHECK_EQ(1, v[1]);
    GGO_CHECK_EQ(3, v[2]);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, pow)
{
  GGO_CHECK_EQ(ggo::pow(4, 0), 1);
  GGO_CHECK_EQ(ggo::pow(4, 1), 4);
  GGO_CHECK_EQ(ggo::pow(4, 2), 16);
  GGO_CHECK_EQ(ggo::pow(-4, 1), -4);
  GGO_CHECK_EQ(ggo::pow(-4, 2), 16);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, fixed_point_div)
{
  GGO_CHECK_EQ(ggo::fixed_point_div<4>(128), 8);
  GGO_CHECK_EQ(ggo::fixed_point_div<4>(-128), -8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(base, round_div)
{
  GGO_CHECK_EQ(ggo::round_div(-9, 4), -2);
  GGO_CHECK_EQ(ggo::round_div(-8, 4), -2);
  GGO_CHECK_EQ(ggo::round_div(-7, 4), -2);
  GGO_CHECK_EQ(ggo::round_div(-6, 4), -2);
  GGO_CHECK_EQ(ggo::round_div(-5, 4), -1);
  GGO_CHECK_EQ(ggo::round_div(-4, 4), -1);
  GGO_CHECK_EQ(ggo::round_div(-3, 4), -1);
  GGO_CHECK_EQ(ggo::round_div(-2, 4), -1);
  GGO_CHECK_EQ(ggo::round_div(-1, 4), 0);
  GGO_CHECK_EQ(ggo::round_div(0, 4), 0);
  GGO_CHECK_EQ(ggo::round_div(1, 4), 0);
  GGO_CHECK_EQ(ggo::round_div(2, 4), 1);
  GGO_CHECK_EQ(ggo::round_div(3, 4), 1);
  GGO_CHECK_EQ(ggo::round_div(4, 4), 1);
  GGO_CHECK_EQ(ggo::round_div(5, 4), 1);
  GGO_CHECK_EQ(ggo::round_div(6, 4), 2);
  GGO_CHECK_EQ(ggo::round_div(7, 4), 2);
  GGO_CHECK_EQ(ggo::round_div(9, 4), 2);
  GGO_CHECK_EQ(ggo::round_div(9, 4), 2);

  GGO_CHECK_EQ(ggo::round_div(-9, -4), 2);
  GGO_CHECK_EQ(ggo::round_div(-8, -4), 2);
  GGO_CHECK_EQ(ggo::round_div(-7, -4), 2);
  GGO_CHECK_EQ(ggo::round_div(-6, -4), 2);
  GGO_CHECK_EQ(ggo::round_div(-5, -4), 1);
  GGO_CHECK_EQ(ggo::round_div(-4, -4), 1);
  GGO_CHECK_EQ(ggo::round_div(-3, -4), 1);
  GGO_CHECK_EQ(ggo::round_div(-2, -4), 1);
  GGO_CHECK_EQ(ggo::round_div(-1, -4), 0);
  GGO_CHECK_EQ(ggo::round_div(0, -4), 0);
  GGO_CHECK_EQ(ggo::round_div(1, -4), 0);
  GGO_CHECK_EQ(ggo::round_div(2, -4), -1);
  GGO_CHECK_EQ(ggo::round_div(3, -4), -1);
  GGO_CHECK_EQ(ggo::round_div(4, -4), -1);
  GGO_CHECK_EQ(ggo::round_div(5, -4), -1);
  GGO_CHECK_EQ(ggo::round_div(6, -4), -2);
  GGO_CHECK_EQ(ggo::round_div(7, -4), -2);
  GGO_CHECK_EQ(ggo::round_div(9, -4), -2);
  GGO_CHECK_EQ(ggo::round_div(9, -4), -2);
}

