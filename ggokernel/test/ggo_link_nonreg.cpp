#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_unordered_pair.h>

GGO_TEST(unordered_pair, equality)
{
  ggo::unordered_pair<int> p1(1, 2);
  ggo::unordered_pair<int> p2(1, 2);
  ggo::unordered_pair<int> p3(2, 1);
  ggo::unordered_pair<int> p4(1, 3);

  GGO_CHECK(p1 == p2);
  GGO_CHECK(p1 == p3);
  GGO_CHECK(p1 != p4);

  char c1 = 'a';
  char c2 = 'b';
  char c3 = 'b'; // Same value but different object.
  ggo::unordered_pair<const char *> p5(&c1, &c2);
  ggo::unordered_pair<const char *> p6(&c1, &c3);
  GGO_CHECK(p5 != p6);
}

GGO_TEST(unordered_pair, find_linked)
{
  std::vector<ggo::unordered_pair<int>> pairs { {1, 2}, {2, 8}, {4, 1}, {2, 5} };

  {
    auto founds = ggo::find_paired(pairs.begin(), pairs.end(), 2);
    GGO_CHECK(std::find(founds.begin(), founds.end(), 1) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 5) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 8) != founds.end());
  }

  {
    auto founds = ggo::find_paired(pairs, 1);
    GGO_CHECK(std::find(founds.begin(), founds.end(), 2) != founds.end());
    GGO_CHECK(std::find(founds.begin(), founds.end(), 4) != founds.end());
  }
}

