#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_2d/ggo_view_transform.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(view_tranform, fit_left_right_padding)
{
  const auto view_rect    = ggo::rect_f::from_left_right_bottom_top(-4, 4, -2, 2);
  const auto render_size  = ggo::size(400, 100);
  const auto transform    = ggo::view_transform<float>::make_fit(view_rect, render_size);

  GGO_CHECK_EQ(transform(ggo::pos2_f{ -4.f, -2.f }), ggo::pos2_f(100, 0));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  4.f,  2.f }), ggo::pos2_f(300, 100));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  0.f,  0.f }), ggo::pos2_f(200, 50));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(view_tranform, fit_bottom_top_padding)
{
  const auto view_rect   = ggo::rect_f::from_left_right_bottom_top(-2, 2, -4, 4);
  const auto render_size = ggo::size(100, 400);
  const auto transform   = ggo::view_transform<float>::make_fit(view_rect, render_size);

  GGO_CHECK_EQ(transform(ggo::pos2_f{ -2.f, -4.f }), ggo::pos2_f(0, 100));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  2.f,  4.f }), ggo::pos2_f(100, 300));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  0.f,  0.f }), ggo::pos2_f(50, 200));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(view_tranform, fill_left_right_padding)
{
  const auto view_rect   = ggo::rect_f::from_left_right_bottom_top(-4, 4, -1, 1);
  const auto render_size = ggo::size(200, 100);
  const auto transform   = ggo::view_transform<float>::make_fill(view_rect, render_size);

  GGO_CHECK_EQ(transform(ggo::pos2_f{ -4.f, -1.f }), ggo::pos2_f(-100, 0));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  4.f,  1.f }), ggo::pos2_f(300, 100));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  0.f,  0.f }), ggo::pos2_f(100, 50));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(view_tranform, fill_bottom_top_padding)
{
  const auto view_rect   = ggo::rect_f::from_left_right_bottom_top(-1, 1, -4, 4);
  const auto render_size = ggo::size(100, 200);
  const auto transform   = ggo::view_transform<float>::make_fill(view_rect, render_size);

  GGO_CHECK_EQ(transform(ggo::pos2_f{ -1.f, -4.f }), ggo::pos2_f(0, -100));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  1.f,  4.f }), ggo::pos2_f(100, 300));
  GGO_CHECK_EQ(transform(ggo::pos2_f{  0.f,  0.f }), ggo::pos2_f(50, 100));
}

