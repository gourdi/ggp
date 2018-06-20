#define DEBUG_MEAN_BOX_FILTER 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_mean_box_filter2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, horizontal_duplicate_edge)
{
  float data[] = { 1.f, 2.f, 3.f, 4.f, 5.f };

  ggo::mean_box_filter_2d_horz<ggo::border_mode::duplicate_edge>(data, data, { 5, 1 }, 2);

  GGO_CHECK_FLOAT_EQ(data[0], 8.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[1], 11.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[2], 3.f);
  GGO_CHECK_FLOAT_EQ(data[3], 19.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[4], 22.f / 5.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, horizontal_mirror)
{
  const float input[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
  float output[] = { 0.f, 0.f, 0.f, 0.f, 0.f};

  ggo::mean_box_filter_2d_horz<ggo::border_mode::mirror>(input, output, { 5, 1 }, 2);

  GGO_CHECK_FLOAT_EQ(output[0], 9.f / 5.f);
  GGO_CHECK_FLOAT_EQ(output[1], 11.f / 5.f);
  GGO_CHECK_FLOAT_EQ(output[2], 3.f);
  GGO_CHECK_FLOAT_EQ(output[3], 19.f / 5.f);
  GGO_CHECK_FLOAT_EQ(output[4], 21.f / 5.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, horizontal_loop)
{
  const float input[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
  float output[] = { 0.f, 0.f, 0.f, 0.f, 0.f };

  ggo::mean_box_filter_2d_horz<ggo::border_mode::loop>(input, output, { 5, 1 }, 2);

  GGO_CHECK_FLOAT_EQ(output[0], 3.f);
  GGO_CHECK_FLOAT_EQ(output[1], 3.f);
  GGO_CHECK_FLOAT_EQ(output[2], 3.f);
  GGO_CHECK_FLOAT_EQ(output[3], 3.f);
  GGO_CHECK_FLOAT_EQ(output[4], 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, horizontal_duplicate_edge_clipping)
{
  float data[] = { 1.f, 2.f, 3.f, 4.f, 5.f };

  ggo::mean_box_filter_2d_horz<ggo::border_mode::duplicate_edge>(data, data, { 5, 1 }, ggo::rect_int::from_left_right_bottom_top(2, 3, 0, 0), 2);

  GGO_CHECK_FLOAT_EQ(data[0], 1.f);
  GGO_CHECK_FLOAT_EQ(data[1], 2.f);
  GGO_CHECK_FLOAT_EQ(data[2], 3.f);
  GGO_CHECK_FLOAT_EQ(data[3], 19.f / 5.f);
  GGO_CHECK_FLOAT_EQ(data[4], 5.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, vertical_duplicate_edge)
{
  float data[] = {
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f };

  ggo::mean_box_filter_2d_vert<ggo::border_mode::duplicate_edge>(data, data, { 4, 3 }, 1);

  GGO_CHECK_FLOAT_EQ(data[0], 0.f);
  GGO_CHECK_FLOAT_EQ(data[4], 0.f);
  GGO_CHECK_FLOAT_EQ(data[8], 0.f);

  GGO_CHECK_FLOAT_EQ(data[1], 2.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[5], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[9], 0.f);

  GGO_CHECK_FLOAT_EQ(data[2], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[6], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[10], 1.f / 3.f);

  GGO_CHECK_FLOAT_EQ(data[3], 0.f);
  GGO_CHECK_FLOAT_EQ(data[7], 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[11], 2.f / 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter, vertical_duplicate_edge_clipping)
{
  float data[] = {
    1.f, 0.f,
    2.f, 0.f,
    4.f, 0.f };

  ggo::mean_box_filter_2d_vert<ggo::border_mode::duplicate_edge>(data, data, { 2, 3 }, ggo::rect_int::from_left_right_bottom_top(0, 0, 1, 2), 1);

  GGO_CHECK_FLOAT_EQ(data[0], 1.f);
  GGO_CHECK_FLOAT_EQ(data[2], 7.f / 3.f);
  GGO_CHECK_FLOAT_EQ(data[4], 10.f / 3.f);

  GGO_CHECK_FLOAT_EQ(data[1], 0.f);
  GGO_CHECK_FLOAT_EQ(data[3], 0.f);
  GGO_CHECK_FLOAT_EQ(data[5], 0.f);
}


