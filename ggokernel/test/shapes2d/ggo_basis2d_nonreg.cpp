#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis2d, rotation)
{
  ggo::basis2d<float> basis({ 2.f, 0.f }, { 1.f, 0.f }, { 0.f, 1.f });
  basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

  GGO_CHECK_FLOAT_EQ(basis.pos().x(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.pos().y(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.x().x(), 0.f);
  GGO_CHECK_FLOAT_EQ(basis.x().y(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.y().x(), -1.f);
  GGO_CHECK_FLOAT_EQ(basis.y().y(), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis2d, local_world)
{
  ggo::basis2d<float> basis({ 2.f, 1.f }, { 3.f, 1.f }, { -1.f, 2.f });

  ggo::pos2f p1 = basis.point_from_local_to_world({ 1.f, 2.f });
  GGO_CHECK_FLOAT_EQ(p1.x(), 3.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 6.f);

  ggo::pos2f p2 = basis.point_from_world_to_local({ 3.f, 6.f });
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 2.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(orthogonal_basis2d, rotation)
{
  ggo::orthogonal_basis2d<float> basis({ 2.f, 0.f }, { 2.f, 0.f });
  basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

  GGO_CHECK_FLOAT_EQ(basis.pos().x(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.pos().y(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.x().x(), 0.f);
  GGO_CHECK_FLOAT_EQ(basis.x().y(), 2.f);
  GGO_CHECK_FLOAT_EQ(basis.y().x(), -2.f);
  GGO_CHECK_FLOAT_EQ(basis.y().y(), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(orthogonal_basis2d, local_world)
{
  ggo::orthogonal_basis2d<float> basis({ 2.f, 1.f }, { 3.f, 1.f });

  ggo::pos2f p1 = basis.point_from_local_to_world({ 1.f, 2.f });
  GGO_CHECK_FLOAT_EQ(p1.x(), 3.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 8.f);

  ggo::pos2f p2 = basis.point_from_world_to_local({ 7.f, 6.f });
  GGO_CHECK_FLOAT_EQ(p2.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(orthonormal_basis2d, rotation)
{
  ggo::orthonormal_basis2d<float> basis({ 2.f, 0.f }, { 2.f, 0.f });
  basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

  GGO_CHECK_FLOAT_EQ(basis.pos().x(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.pos().y(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.x().x(), 0.f);
  GGO_CHECK_FLOAT_EQ(basis.x().y(), 1.f);
  GGO_CHECK_FLOAT_EQ(basis.y().x(), -1.f);
  GGO_CHECK_FLOAT_EQ(basis.y().y(), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(orthonormal_basis2d, local_world)
{
  ggo::orthonormal_basis2d<float> basis({ 2.f, 1.f }, { 0.f, 1.f });

  ggo::pos2f p1 = basis.point_from_local_to_world({ 3.f, 1.f });
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 4.f);

  ggo::pos2f p2 = basis.point_from_world_to_local({ 1.f, 4.f });
  GGO_CHECK_FLOAT_EQ(p2.x(), 3.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.0f);
}
