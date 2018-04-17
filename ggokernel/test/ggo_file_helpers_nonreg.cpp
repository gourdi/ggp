#include <kernel/ggo_file_helpers.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(file_helpers, extension)
{
  {
    auto ext = ggo::get_file_extension("test.txt");
    GGO_CHECK_EQ(ext, "txt");
  }

  {
    auto ext = ggo::get_file_extension("test.test.txt");
    GGO_CHECK_EQ(ext, "txt");
  }

  {
    auto ext = ggo::get_file_extension("test");
    GGO_CHECK_EQ(ext, "");
  }
}
