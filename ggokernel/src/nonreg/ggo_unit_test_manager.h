#ifndef __GGO_UNIT_TEST_MANAGER__
#define __GGO_UNIT_TEST_MANAGER__

#include <string>
#include <map>
#include <vector>

namespace ggo
{
  class unit_test;

  class unit_test_manager
  {
  public:

    unit_test_manager(const unit_test_manager &) = delete;
    unit_test_manager&  operator=(const unit_test_manager &) = delete;

    static  unit_test_manager & instance();

    void                    add_test(const std::string & test_case_name, unit_test * unit_test);
    void                    run_all(int argc, char ** argv);

  private:

    unit_test_manager() {}

    std::map<std::string, std::vector<unit_test *>> _tests;
  };
}


#endif

