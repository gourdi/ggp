#include <ggo_nonreg.h>
#include <ggo_tree3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(trees, tree3d)
{
  std::vector<std::pair<ggo::point3d_float, void*>> points{
    { {  2.f, -1.f, 0.f }, nullptr },
    { {  1.f,  1.f, 0.f }, nullptr },
    { { -1.f,  3.f, 0.f }, nullptr },
    { {  4.f,  4.f, 0.f }, nullptr },
    { {  3.f,  5.f, 0.f }, nullptr },
    { {  0.f,  4.f, 0.f }, nullptr }, };

  ggo::tree3d<void*> tree(points);

  tree.dump(std::cout);

  auto result = tree.find_points({ 3.0f, 4.0f, 0.0f }, 2.f);
}

