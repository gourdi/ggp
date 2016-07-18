#include <ggo_nonreg.h>
#include <ggo_kdtree.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(trees, kdtree)
{
  using tree_type = ggo::kdtree<void *, 2>;
  std::vector<tree_type::data_point> points{
    { {  2.f, -1.f }, nullptr },
    { {  1.f,  1.f }, nullptr },
    { { -1.f,  3.f }, nullptr },
    { {  4.f,  4.f }, nullptr },
    { {  3.f,  5.f }, nullptr },
    { {  0.f,  4.f }, nullptr }, };

  tree_type tree(points);

  auto result = tree.find_points({ 3.0f, 4.0f }, 2.f);
  GGO_CHECK_EQ(result.size(), 2);
}

