#ifndef __GGO_UNIT_TEST_MANAGER__
#define __GGO_UNIT_TEST_MANAGER__

#include <string>
#include <map>
#include <vector>

class ggo_unit_test;

class ggo_unit_test_manager
{
public:

                                  ggo_unit_test_manager(const ggo_unit_test_manager &) = delete;
          ggo_unit_test_manager&  operator=(const ggo_unit_test_manager &) = delete;
  
  static  ggo_unit_test_manager & instance();

          void                    add_test(const std::string & test_case_name, ggo_unit_test * unit_test);
          void                    run_all(int argc, char ** argv);
  
private:

                                  ggo_unit_test_manager() {}

  std::map<std::string, std::vector<ggo_unit_test *>> _tests;
};

#endif

