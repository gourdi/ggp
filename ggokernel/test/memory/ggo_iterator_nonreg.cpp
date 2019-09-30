#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_iterator.h>
#include <array>

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, static_offset_input)
{
  std::array<int, 5> a{ {1, 2, 3, 4, 5} };

  ggo::input_iterator_t it(a.data());
  GGO_CHECK_EQ(*it, 1);
  GGO_CHECK_EQ(it.get(2), 3);
  GGO_CHECK(it == ggo::input_iterator_t(a.data()));

  auto it2 = it + 3;
  GGO_CHECK_EQ(*it2, 4);

  GGO_CHECK_EQ(ggo::distance(it, it2), 3);

  auto it3 = it2 - 1;
  GGO_CHECK_EQ(*it3, 3)
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, static_offset_input_stride)
{
  const std::array<int, 5> a{ {1, 2, 3, 4, 5} };

  ggo::input_iterator_t<int, 2> it(a.data());

  GGO_CHECK_EQ(it.get(2), 5);

  auto it2 = it + 2;
  GGO_CHECK_EQ(*it2, 5);

  GGO_CHECK_EQ(ggo::distance(it, it2), 2);

  auto it3 = it2 - 1;
  GGO_CHECK_EQ(*it3, 3)
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, static_offset_output)
{
  std::array<int, 5> a{ {0, 0, 0, 0, 0} };

  ggo::output_iterator_t it(a.data());
  it << 1 << 2 << 3 << 4 << 5;
  GGO_CHECK_EQ(a[0], 1);
  GGO_CHECK_EQ(a[1], 2);
  GGO_CHECK_EQ(a[2], 3);
  GGO_CHECK_EQ(a[3], 4);
  GGO_CHECK_EQ(a[4], 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, input)
{
  const std::array<int, 5> a{ {1, 2, 3, 4, 5} };

  ggo::input_iterator it(a.data(), 1);
  GGO_CHECK_EQ(*it, 1);
  GGO_CHECK_EQ(it.get(2), 3);
  GGO_CHECK(it == ggo::input_iterator(a.data(), 1));

  auto it2 = it + 3;
  GGO_CHECK_EQ(*it2, 4);

  GGO_CHECK_EQ(ggo::distance(it, it2), 3);

  auto it3 = it2 - 1;
  GGO_CHECK_EQ(*it3, 3)
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, input_offset)
{
  const std::array<int, 5> a{ {1, 2, 3, 4, 5} };

  ggo::input_iterator it(a.data(), 2);

  auto it2 = it + 2;
  GGO_CHECK_EQ(*it2, 5);

  GGO_CHECK_EQ(ggo::distance(it, it2), 2);

  auto it3 = it2 - 1;
  GGO_CHECK_EQ(*it3, 3)
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(iterator, output)
{
  std::array<int, 5> a{ {0, 0, 0, 0, 0} };

  ggo::output_iterator it(a.data(), 1);
  it << 1 << 2 << 3 << 4 << 5;
  GGO_CHECK_EQ(a[0], 1);
  GGO_CHECK_EQ(a[1], 2);
  GGO_CHECK_EQ(a[2], 3);
  GGO_CHECK_EQ(a[3], 4);
  GGO_CHECK_EQ(a[4], 5);
}


