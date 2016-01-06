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
  field._shape.center() = ggo::point2d_float(1, 1);
  
  GGO_CHECK_FABS(field.evaluate(1, 0), 1);
  GGO_CHECK_FABS(field.evaluate(3, -3), 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scalar_field2d, multi_fields2d)
{
  ggo::multi_scalar_field2d<ggo::gaussian_field2d, float, ggo::array> multi_fields1;
  multi_fields1._fields.set_size(2);
  multi_fields1._fields[0]._x = 1;
  multi_fields1._fields[0]._y = -2;
  multi_fields1._fields[0]._var = 1;
  multi_fields1._fields[0]._amp = 2;
  multi_fields1._fields[1]._x = -2;
  multi_fields1._fields[1]._y = 3;
  multi_fields1._fields[1]._var = 2;
  multi_fields1._fields[1]._amp = 1;
  
  ggo::multi_scalar_field2d<ggo::gaussian_field2d> multi_fields2;
  multi_fields2._fields.resize(2);
  multi_fields2._fields[0]._x = 1;
  multi_fields2._fields[0]._y = -2;
  multi_fields2._fields[0]._var = 1;
  multi_fields2._fields[0]._amp = 2;
  multi_fields2._fields[1]._x = -2;
  multi_fields2._fields[1]._y = 3;
  multi_fields2._fields[1]._var = 2;
  multi_fields2._fields[1]._amp = 1;
  
  GGO_CHECK_FABS(multi_fields1.evaluate(0, 0), multi_fields2.evaluate(0, 0));
  GGO_CHECK_FABS(multi_fields1.evaluate(1, 2), multi_fields2.evaluate(1, 2));
  GGO_CHECK_FABS(multi_fields1.evaluate(4, 8), multi_fields2.evaluate(4, 8));
}