#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_mean_box_filter.h>
#include <kernel/memory/ggo_buffer_iterator.h>
#include <vector>

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, 32f_heaviside)
{
  std::vector<float> data{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f };

  auto it = ggo::make_read_write_iterator(data.data());

  auto left   = [&](int x) { GGO_CHECK(x < 0); return 1.f; };
  auto right  = [&](int x) { GGO_CHECK(x >= data.size()); return 1.f; };
  auto divide = [&](float sum) { return sum / 5.f; };

  ggo::mean_box_filter(it, left, right, data.size(), divide, 2);

  GGO_CHECK_FLOAT_EQ(data[0], 8.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[1], 9.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[2], 2.f);
  GGO_CHECK_FLOAT_EQ(data[3], 2.f);
  GGO_CHECK_FLOAT_EQ(data[4], 9.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[5], 8.f / 5.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, 32f_ramp)
{
  std::vector<float> data{ 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };

  auto it = ggo::make_read_write_iterator(data.data());

  auto left = [&](int x) {
    switch (x)
    {
    case -1: return 2.f;
    case -2: return 1.f;
    default: GGO_CHECK(false); return 0.f;
    }
  };

  auto right = [&](int x) {
    switch (x)
    {
    case 6: return 9.f;
    case 7: return 10.f;
    default: GGO_CHECK(false); return 0.f;
    }
  };

  auto divide = [&](float sum) { return sum / 5.f; };

  ggo::mean_box_filter(it, left, right, data.size(), divide, 2);

  GGO_CHECK_FLOAT_EQ(data[0], 3.f);
  GGO_CHECK_FLOAT_EQ(data[1], 4.f);
  GGO_CHECK_FLOAT_EQ(data[2], 5.f);
  GGO_CHECK_FLOAT_EQ(data[3], 6.f);
  GGO_CHECK_FLOAT_EQ(data[4], 7.f);
  GGO_CHECK_FLOAT_EQ(data[5], 8.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, 8u_fixed_point)
{
  std::vector<uint8_t> data{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

  auto it = ggo::make_cast_read_write_iterator<uint16_t, ggo::cast_mode::regular, ggo::cast_mode::regular>(data.data());

  auto left = [&](int x) -> uint16_t { GGO_CHECK(x < 0); return 0; };
  auto right = [&](int x) -> uint16_t { GGO_CHECK(x >= data.size()); return 0xff; };

  auto divide = [&](uint16_t sum) { return ggo::round_div<uint16_t>(sum, 7); };

  ggo::mean_box_filter(it, left, right, data.size(), divide, 3);

  GGO_CHECK_FLOAT_EQ(data[0], 0x92);
  GGO_CHECK_FLOAT_EQ(data[1], 0xb6);
  GGO_CHECK_FLOAT_EQ(data[2], 0xdb);
  GGO_CHECK_FLOAT_EQ(data[3], 0xff);
  GGO_CHECK_FLOAT_EQ(data[4], 0xff);
  GGO_CHECK_FLOAT_EQ(data[5], 0xff);
  GGO_CHECK_FLOAT_EQ(data[6], 0xff);
  GGO_CHECK_FLOAT_EQ(data[7], 0xff);
}


