#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <kernel/trees/ggo_octree.h>

namespace 
{
  template <typename shape_t>
  ggo::octree<shape_t, typename shape_t::type> build_octree(std::initializer_list<shape_t> shapes, int depth = 3)
  {
    using data_t = typename shape_t::type;

    auto get_bounding_box = [](const shape_t & shape)
    {
      auto aabb = shape.get_bounding_box(ggo::basis3d<data_t>());
      if (aabb.has_value() == false)
      {
        throw std::runtime_error("could not get bounding box");
      }
      return *aabb;
    };

    return ggo::octree<shape_t, typename shape_t::type>(shapes.begin(), shapes.end(), get_bounding_box, depth);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(octree, single_object)
{
  const ggo::sphere3d_f sphere({ 2.f, 2.f, 0.f }, 1.f);
  auto tree = build_octree({ sphere });

  GGO_CHECK_EQ(tree.get_depth(), 1);
  GGO_CHECK_EQ(tree.bounding_box(), *sphere.get_bounding_box(ggo::basis3d_f()));
  GGO_CHECK_EQ(tree.data().size(), 1);
  GGO_CHECK_EQ(tree.data()[0], sphere);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(octree, two_non_separable_objects)
{
  const float sqrt2 = std::sqrt(2.0f);
  const ggo::sphere3d_f sphere1({ 1.f, 1.f, 0.f }, sqrt2);
  const ggo::sphere3d_f sphere2({ -1.f, -1.f, 0.f }, sqrt2);
  auto tree = build_octree({ sphere1, sphere2 });

  GGO_CHECK_EQ(tree.get_depth(), 1);
  GGO_CHECK_EQ(tree.bounding_box(), ggo::box3d_data_float(-1 - sqrt2, 1 + sqrt2, -1 - sqrt2, 1 + sqrt2, -sqrt2, sqrt2));
  GGO_CHECK_EQ(tree.data().size(), 2);
  GGO_CHECK(tree.data()[0] == sphere1 || tree.data()[0] == sphere2);
  GGO_CHECK(tree.data()[1] == sphere1 || tree.data()[1] == sphere2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(octree, two_separable_objects)
{
  const ggo::sphere3d_f sphere1({ 2.f, 2.f, 0.f }, 1.f);
  const ggo::sphere3d_f sphere2({ -2.f, -2.f, 0.f }, 1.f);
  auto tree = build_octree({ sphere1, sphere2 });

  GGO_CHECK_EQ(tree.get_depth(), 2);
  GGO_CHECK_EQ(tree.bounding_box(), ggo::box3d_data_float(-3, 3, -3, 3, -1, 1));
  GGO_CHECK(tree.data().empty());
  
  GGO_CHECK(
    tree[0].bounding_box() == sphere1.get_bounding_box(ggo::basis3d_f()) ||
    tree[0].bounding_box() == sphere2.get_bounding_box(ggo::basis3d_f()));
  GGO_CHECK_EQ(tree[0].data().size(), 1);
  GGO_CHECK_EQ(tree[1].data().size(), 1);
  GGO_CHECK(tree[0].data()[0] == sphere1 || tree[1].data()[0] == sphere2);
  GGO_CHECK(tree[0].data()[0] == sphere1 || tree[1].data()[0] == sphere2);
}
