#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/lcp/lemke.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(lemke, case1)
{
  // Positive constant.
  {
    const ggo::array<float, 2> M({ { 2 } });

    const ggo::array<float, 2> q({ { 3.f } }); 

    ggo::array<float, 2> w;
    ggo::array<float, 2> z;
    ggo::lemke::solve(M, q, w, z);

    GGO_ASSERT_FLOAT_EQ(w(0, 0), 3.f);
    GGO_ASSERT_FLOAT_EQ(z(0, 0), 0.f);
  }

  // Positive constant.
  {
    const ggo::array<float, 2> M({ { 2 } });

    const ggo::array<float, 2> q({ { -3.f } });

    ggo::array<float, 2> w;
    ggo::array<float, 2> z;
    ggo::lemke::solve(M, q, w, z);

    GGO_ASSERT_FLOAT_EQ(w(0, 0), 0.f);
    GGO_ASSERT_FLOAT_EQ(z(0, 0), 3.f / 2.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(lemke, case2)
{
  const ggo::array<float, 2> M({
    { 0.f, 0.f, 1.f },
    { 0.f, 0.f, 1.f },
    { -1.f, -1.f, 0.f } });

  const ggo::array<float, 2> q({
    { -2.f },
    { -1.f },
    { 3.f } });

  ggo::array<float, 2> w;
  ggo::array<float, 2> z;
  ggo::lemke::solve(M, q, w, z);

  GGO_ASSERT_FLOAT_EQ(w(0, 0), 0.f);
  GGO_ASSERT_FLOAT_EQ(w(0, 1), 1.f);
  GGO_ASSERT_FLOAT_EQ(w(0, 2), 0.f);

  GGO_ASSERT_FLOAT_EQ(z(0, 0), 3.f);
  GGO_ASSERT_FLOAT_EQ(z(0, 1), 0.f);
  GGO_ASSERT_FLOAT_EQ(z(0, 2), 2.f);
}
