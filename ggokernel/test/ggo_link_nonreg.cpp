#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_link.h>

GGO_TEST(link, equality)
{
  ggo::link<int> link1(1, 2);
  ggo::link<int> link2(1, 2);
  ggo::link<int> link3(2, 1);
  ggo::link<int> link4(1, 3);

  GGO_CHECK(link1 == link2);
  GGO_CHECK(link1 == link3);
  GGO_CHECK(link1 != link4);

  char c1 = 'a';
  char c2 = 'b';
  char c3 = 'b'; // Same value but different object.
  ggo::link<const char *> link5(&c1, &c2);
  ggo::link<const char *> link6(&c1, &c3);
  GGO_CHECK(link5 != link6);
}

GGO_TEST(link, find_linked)
{
  std::vector<ggo::link<int>> links { {1, 2}, {2, 8}, {4, 1}, {2, 5} };

  {
    auto founds = ggo::find_linked(links.begin(), links.end(), 2);
    GGO_CHECK(std::find(founds.begin(), founds.end(), 1) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 5) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 8) != founds.end());
  }

  {
    auto founds = ggo::find_linked(links, 1);
    GGO_CHECK(std::find(founds.begin(), founds.end(), 2) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 4) != founds.end());
  }
}

