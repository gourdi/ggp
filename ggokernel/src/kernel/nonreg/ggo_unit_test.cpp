#include "ggo_unit_test.h"
#include "ggo_unit_test_manager.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  unit_test::unit_test(const char * test_case_name, const char * unit_test_name)
    :
    _name(unit_test_name)
  {
    unit_test_manager::instance().add_test(test_case_name, this);
  }
}
