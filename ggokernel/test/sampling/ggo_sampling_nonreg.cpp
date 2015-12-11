#ifdef WIN32
#pragma warning(disable: 4305) // possible loss of data
#pragma warning(disable: 4244) // possible loss of data
#endif

#include "../ggo_kernel_nonreg.h"
#include <ggo_halton.h>
#include <ggo_best_candidate_sequence.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sampling, disc)
{
  // Delta sampling.
  {
    ggo::disc_float disc(0, 0, 2);
    auto samples = disc.uniform_sampling(1);

    GGO_CHECK(samples.size() == 12);

    GGO_CHECK(find_point(samples, 0.5, 0.5));
    GGO_CHECK(find_point(samples, 0.5, -0.5));
    GGO_CHECK(find_point(samples, -0.5, 0.5));
    GGO_CHECK(find_point(samples, -0.5, -0.5));

    GGO_CHECK(find_point(samples, 1.5, 0.5));
    GGO_CHECK(find_point(samples, 1.5, -0.5));
    GGO_CHECK(find_point(samples, -1.5, 0.5));
    GGO_CHECK(find_point(samples, -1.5, -0.5));

    GGO_CHECK(find_point(samples, 0.5, 1.5));
    GGO_CHECK(find_point(samples, 0.5, -1.5));
    GGO_CHECK(find_point(samples, -0.5, 1.5));
    GGO_CHECK(find_point(samples, -0.5, -1.5));
  }

  {
    ggo::disc_float disc(2.5, 1.5, 1);
    auto samples = disc.uniform_sampling(1);

    GGO_CHECK(samples.size() == 4);

    GGO_CHECK(find_point(samples, 3, 2));
    GGO_CHECK(find_point(samples, 3, 1));
    GGO_CHECK(find_point(samples, 2, 2));
    GGO_CHECK(find_point(samples, 2, 1));
  }

  // Samples count sampling.
  {
    ggo::disc_float disc(4, 3, 1);
    auto samples = disc.minimum_uniform_sampling(3, 1000);

    GGO_CHECK(samples.size() == 4);

    GGO_CHECK(find_point(samples, 4 + 1 / std::sqrt(2.f), 3 + 1 / std::sqrt(2.f)));
    GGO_CHECK(find_point(samples, 4 + 1 / std::sqrt(2.f), 3 - 1 / std::sqrt(2.f)));
    GGO_CHECK(find_point(samples, 4 - 1 / std::sqrt(2.f), 3 + 1 / std::sqrt(2.f)));
    GGO_CHECK(find_point(samples, 4 - 1 / std::sqrt(2.f), 3 - 1 / std::sqrt(2.f)));
  }

  {
    ggo::disc_float disc(-2, 5, 7);
    auto samples = disc.minimum_uniform_sampling(12345);

    GGO_CHECK(samples.size() == 12352);

    for (const auto & sample : samples)
    {
      GGO_CHECK(disc.is_point_inside(sample) == true);
    }
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sampling, rect)
{
  {
    ggo::rect_float rect { 1.5, 0.5, 2, 4 };

    auto samples = rect.uniform_sampling(1);

    GGO_CHECK(samples.size() == 8);
    GGO_CHECK(find_point(samples, ggo::point2d_float(2, 1)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(2, 2)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(2, 3)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(2, 4)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3, 1)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3, 2)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3, 3)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3, 4)));
  }

  {
    ggo::rect_float rect{ 1, 1, 3, 2 };

    auto samples = rect.minimum_uniform_sampling(3);

    GGO_CHECK(samples.size() == 4);
    GGO_CHECK(find_point(samples, ggo::point2d_float(1.5, 1)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3.5, 1)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(1.5, 3)));
    GGO_CHECK(find_point(samples, ggo::point2d_float(3.5, 3)));
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
    float dist = ggo::distance(x, y);
    GGO_CHECK(dist <= 0.5f);
  }
}
