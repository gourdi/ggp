#include "ggo_kernel_nonreg.h"
#include <ggo_array.h>
#include <ggo_multi_scalar_field_2d.h>
#include <ggo_shape_field_2d.h>
#include <ggo_gaussian_field_2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scalar_field2d, shape_field2d)
{
  ggo::shape_field2d<ggo::disc> field;
  
  field._shape.radius() = 2;
  field._shape.center() = ggo::pos2f(1.f, 1.f);
  
  GGO_CHECK_FLOAT_EQ(field.evaluate(1, 0), 1);
  GGO_CHECK_FLOAT_EQ(field.evaluate(3, -3), 0);
}
