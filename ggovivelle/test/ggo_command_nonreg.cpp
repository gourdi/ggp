#include <ggo_nonreg.h>
#include <ggo_command.h>

//////////////////////////////////////////////////////////////
GGO_TEST(command, regular)
{
  {
    auto command = ggo::parse_command("test");
    GGO_CHECK_EQ(command._name, "test");
    GGO_CHECK(command._parameters.empty());
  }

  {
    auto command = ggo::parse_command("test:a=b");
    GGO_CHECK_EQ(command._name, "test");
    GGO_CHECK_EQ(command._parameters.size(), 1);
    GGO_CHECK_EQ(command._parameters["a"], "b");
  }
}

//////////////////////////////////////////////////////////////
GGO_TEST(command, filename)
{
  {
    auto command = ggo::parse_command("test", true);
    GGO_CHECK_EQ(command._name, "test");
    GGO_CHECK(command._parameters.empty());
  }

  {
    auto command = ggo::parse_command("test:a=b", true);
    GGO_CHECK_EQ(command._name, "test");
    GGO_CHECK_EQ(command._parameters.size(), 1);
    GGO_CHECK_EQ(command._parameters["a"], "b");
  }

  {
    auto command = ggo::parse_command("d:/test:a=b", true);
    GGO_CHECK_EQ(command._name, "d:/test");
    GGO_CHECK_EQ(command._parameters.size(), 1);
    GGO_CHECK_EQ(command._parameters["a"], "b");
  }
}


