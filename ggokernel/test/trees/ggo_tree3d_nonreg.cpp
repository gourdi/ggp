#include <ggo_nonreg.h>
#include <ggo_tree3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(trees, tree3d)
{
  std::vector<ggo::point3d_float> points{
    {  2.f, -1.f, 0.f },
    {  1.f,  1.f, 0.f },
    { -1.f,  3.f, 0.f },
    {  4.f,  4.f, 0.f },
    {  3.f,  5.f, 0.f },
    {  0.f,  4.f, 0.f } };

  ggo::tree3d tree(points);

  tree.dump(std::cout);

  auto result = tree.find_points({ 3.0f, 4.0f, 0.0f }, 2.f);
}

