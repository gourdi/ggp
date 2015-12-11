#ifndef __GGO_UNIT_TEST__
#define __GGO_UNIT_TEST__

#include <string>

class ggo_unit_test
{
public:

                              ggo_unit_test(const char * test_case_name, const char * unit_test_name);
  
  virtual void                run_test() = 0;
  
          int                 get_failed_count() const { return _failed_count; }
          const std::string & get_name() const { return _name; }
  
protected:

  int         _failed_count = 0;
  std::string _name;
};

#endif

