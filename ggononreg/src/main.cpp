#include <ggo_nonreg.h>
#include <regex>

GGO_TEST(toto, tata1)
{
  GGO_CHECK(true);
}

GGO_TEST(toto, tata2)
{
  GGO_CHECK_FABS(2, 2);
}

int main(int argc, char ** argv)
{
  try
  {
    std::vector<std::string> strings {"test", "test.", "test.test"};
    
    for (const auto & s : strings)
    {
      std::cout << s << " : " << std::regex_match(s, std::regex("test")) << std::endl;
    }
  }
  catch (std::exception e)
  {
    std::cout << e.what() << std::endl;
  }
  
  
  //RUN_ALL_TESTS();
  
  return 0;
}
