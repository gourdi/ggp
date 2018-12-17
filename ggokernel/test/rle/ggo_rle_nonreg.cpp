#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/rle/ggo_rle.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(rle, same_value)
{
  auto rle_data = ggo::rle_encode({ 1, 1, 1, 1 });
  GGO_CHECK(rle_data[0].first == 1);
  GGO_CHECK(rle_data[0].second == 4);

  ggo::array_i decoded(4);
  ggo::rle_decode(rle_data, decoded);
  GGO_CHECK(decoded(0) == 1);
  GGO_CHECK(decoded(1) == 1);
  GGO_CHECK(decoded(2) == 1);
  GGO_CHECK(decoded(3) == 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rle, single_values)
{
  auto rle_data = ggo::rle_encode({ 1, 3, 2, 4 });
  GGO_CHECK(rle_data[0].first == 1);
  GGO_CHECK(rle_data[0].second == 1);
  GGO_CHECK(rle_data[1].first == 3);
  GGO_CHECK(rle_data[1].second == 1);
  GGO_CHECK(rle_data[2].first == 2);
  GGO_CHECK(rle_data[2].second == 1);
  GGO_CHECK(rle_data[3].first == 4);
  GGO_CHECK(rle_data[3].second == 1);

  ggo::array_i decoded(4);
  ggo::rle_decode(rle_data, decoded);
  GGO_CHECK(decoded(0) == 1);
  GGO_CHECK(decoded(1) == 3);
  GGO_CHECK(decoded(2) == 2);
  GGO_CHECK(decoded(3) == 4);
}

