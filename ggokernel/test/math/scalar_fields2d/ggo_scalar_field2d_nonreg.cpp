#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/scalar_fields_2d/ggo_shape_field_2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scalar_field2d, shape_field2d)
{
  ggo::shape_field2d<ggo::disc_float> field;
  
  field._shape.radius() = 2;
  field._shape.center() = ggo::pos2f(1.f, 1.f);
  
  GGO_CHECK_FLOAT_EQ(field.sample(1, 0), 1);
  GGO_CHECK_FLOAT_EQ(field.sample(3, -3), 0);
}
