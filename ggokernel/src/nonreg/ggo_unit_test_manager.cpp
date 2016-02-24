#include "ggo_unit_test_manager.h"
#include "ggo_unit_test.h"
#include "ggo_color_stream.h"
#include <iostream>
#include <regex>

namespace
{
  /////////////////////////////////////////////////////////////////////
  bool bypass_test_case(const std::string & test_case_name, const std::vector<std::string> & args)
  {
    if (args.empty() == true)
    {
      return false;
    }
    
    for (const auto arg : args)
    {
      if (test_case_name == arg)
      {
        return false;
      }
      
      std::string regex_str = std::string("^") + test_case_name + "\\.\\w*";
      if (std::regex_match(arg, std::regex(regex_str)) == true)
      {
        return false;
      }
    }
    
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////
  bool bypass_unit_test(const std::string & test_case_name, const std::string & unit_test_name, const std::vector<std::string> & args)
  {
    if (args.empty() == true)
    {
      return false;
    }
    
    for (const auto arg : args)
    {
      if (arg == test_case_name)
      {
        return false;
      }
      
      if (test_case_name + '.' + unit_test_name == arg)
      {
        return false;
      }
    }
    
    return true;
  }
}

/////////////////////////////////////////////////////////////////////
ggo_unit_test_manager & ggo_unit_test_manager::instance()
{
  static ggo_unit_test_manager instance;
  
  return instance;
}

/////////////////////////////////////////////////////////////////////
void ggo_unit_test_manager::add_test(const std::string & test_case_name, ggo_unit_test * unit_test)
{
  _tests[test_case_name].push_back(unit_test);
}

/////////////////////////////////////////////////////////////////////
void ggo_unit_test_manager::run_all(int argc, char ** argv)
{
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i)
  {
    args.push_back(argv[i]);
  }

  int unit_test_count = 0;
  int failed_count = 0;
  int bypassed_count = 0;
  
  for (auto & it_map : _tests)
  {
    if (bypass_test_case(it_map.first, args) == true)
    {
      bypassed_count += static_cast<int>(it_map.second.size());
      continue; 
    }
    
    {
      ggo::color_stream cs(ggo::console_color::YELLOW); // Must be scoped.
      cs << "Test case: " << it_map.first << std::endl;
    }
    
    for (auto unit_test : it_map.second)
    {
      if (bypass_unit_test(it_map.first, unit_test->get_name(), args) == true)
      {
        ++bypassed_count;
        continue;
      }
      
      std::cout << "  Unit test: " << unit_test->get_name() << std::endl;
      unit_test->run_test();
      
      failed_count += unit_test->get_failed_count();
      ++unit_test_count;
    }
  }
  
  if (bypassed_count > 0)
  {
    ggo::color_stream cs(ggo::console_color::YELLOW);
    std::cout << std::endl << std::endl << "*** BYPASSED " << bypassed_count << " TEST(S) ***" << std::endl;
  }
  
  if (failed_count == 0)
  {
    ggo::color_stream cs(ggo::console_color::GREEN);
    cs << std::endl << std::endl << "*** NON-REGRESSION SUCCESSFUL: " << unit_test_count << " TEST(S) PASSED ***" << std::endl << std::endl;
  }
  else
  {
    ggo::color_stream cs(ggo::console_color::RED);
    cs << std::endl << std::endl << "*** NON-REGRESSION SUCCESSFUL: " << unit_test_count << " TEST(S) PASSED ***" << std::endl << std::endl;
  }
}
