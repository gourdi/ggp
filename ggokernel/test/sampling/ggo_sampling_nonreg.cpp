#include "../ggo_kernel_nonreg.h"
#include <kernel/ggo_vec2.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/sampling/shape_sampling/ggo_grid_sampling.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_halton.h>
#include <kernel/math/sampling/low_discrepancy_sequences/ggo_best_candidate_sequence.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sampling, disc)
{
  // Delta sampling.
  {
    ggo::disc_f disc({ 0, 0 }, 2);
    auto samples = ggo::grid_sampling(disc, 1.f);

    GGO_CHECK(samples.size() == 12);

    GGO_CHECK(find(samples, { 0.5f, 0.5f }));
    GGO_CHECK(find(samples, { 0.5f, -0.5f }));
    GGO_CHECK(find(samples, { -0.5f, 0.5f }));
    GGO_CHECK(find(samples, { -0.5f, -0.5f }));

    GGO_CHECK(find(samples, { 1.5f, 0.5f }));
    GGO_CHECK(find(samples, { 1.5f, -0.5f }));
    GGO_CHECK(find(samples, { -1.5f, 0.5f }));
    GGO_CHECK(find(samples, { -1.5f, -0.5f }));

    GGO_CHECK(find(samples, { 0.5f, 1.5f }));
    GGO_CHECK(find(samples, { 0.5f, -1.5f }));
    GGO_CHECK(find(samples, { -0.5f, 1.5f }));
    GGO_CHECK(find(samples, { -0.5f, -1.5f }));
  }

  {
    ggo::disc_f disc({ 2.5, 1.5 }, 1);
    auto samples = ggo::grid_sampling(disc, 1.f);

    GGO_CHECK(samples.size() == 4);

    GGO_CHECK(find_point(samples, { 3, 2 }));
    GGO_CHECK(find_point(samples, { 3, 1 }));
    GGO_CHECK(find_point(samples, { 2, 2 }));
    GGO_CHECK(find_point(samples, { 2, 1 }));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sampling, rect)
{
  {
    auto rect = ggo::rect_f::from_left_bottom_width_height(1.5, 0.5, 2, 4);

    auto samples = grid_sampling(rect, 1.f);

    GGO_CHECK(samples.size() == 8);
    GGO_CHECK(find_point(samples, ggo::pos2_f(2.f, 1.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(2.f, 2.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(2.f, 3.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(2.f, 4.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(3.f, 1.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(3.f, 2.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(3.f, 3.f)));
    GGO_CHECK(find_point(samples, ggo::pos2_f(3.f, 4.f)));
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(sampling, halton)
{
  int count_rect = sizeof(ggo::halton_rect_2d_table_2_3) / sizeof(ggo::halton_rect_2d_table_2_3[0]);
  GGO_CHECK(count_rect == 1000);
  
  for (int i = 0; i < count_rect; ++i)
  {
    GGO_CHECK(std::abs(ggo::halton_rect_2d_table_2_3[i].x()) <= 0.5f);
    GGO_CHECK(std::abs(ggo::halton_rect_2d_table_2_3[i].y()) <= 0.5f);
  }
  
  int count_disc = sizeof(ggo::halton_disc_2d_table_2_3) / sizeof(ggo::halton_disc_2d_table_2_3[0]);
  GGO_CHECK(count_disc == 1000);
  
  for (int i = 0; i < count_disc; ++i)
  {
    float x = ggo::halton_disc_2d_table_2_3[i].x();
    float y = ggo::halton_disc_2d_table_2_3[i].y();
    float dist = ggo::length(ggo::vec2_f(x, y));
    GGO_CHECK(dist <= 0.5f);
  }
}
