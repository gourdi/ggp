#include <kernel/scan/ggo_scan.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(scan, scan_rows_up)
{
  ggo::scan_rows_up scan;

  std::vector<ggo::vec2_i> scanned;
  for (auto [x, y] : scan(ggo::rect_int::from_left_right_bottom_top(1, 3, 4, 5)))
  {
    scanned.emplace_back(x, y);
  }

  const std::vector<ggo::vec2_i> expected({
    { 1, 4 }, { 2, 4 }, { 3, 4 },
    { 1, 5 }, { 2, 5 }, { 3, 5 } });

  GGO_CHECK_EQ(scanned, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scan, scan_rows_down)
{
  ggo::scan_rows_down scan;

  std::vector<ggo::vec2_i> scanned;
  for (auto [x, y] : scan(ggo::rect_int::from_left_right_bottom_top(1, 3, 4, 5)))
  {
    scanned.emplace_back(x, y);
  }

  const std::vector<ggo::vec2_i> expected({
    { 1, 5 }, { 2, 5 }, { 3, 5 },
    { 1, 4 }, { 2, 4 }, { 3, 4 } });

  GGO_CHECK_EQ(scanned, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scan, scan_tiles_up_single_tile)
{
  std::vector<ggo::rect_int> tiles;

  ggo::scan_tiles_up scan;
  scan(ggo::rect_int::from_left_right_bottom_top(5, 15, 10, 20), 40, 40, [&](const ggo::rect_int& tile)
  {
    tiles.push_back(tile);
  });

  const std::vector<ggo::rect_int> expected_tiles({
    ggo::rect_int::from_left_right_bottom_top(5, 15, 10, 20) });
  GGO_CHECK_EQ(tiles, expected_tiles);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scan, scan_tiles_up_multiple_tiles)
{
  std::vector<ggo::rect_int> tiles;

  ggo::scan_tiles_up scan;
  scan(ggo::rect_int::from_left_right_bottom_top(5, 12, 10, 15), 5, 5, [&](const ggo::rect_int& tile)
  {
    tiles.push_back(tile);
  });

  const std::vector<ggo::rect_int> expected_tiles({
    ggo::rect_int::from_left_right_bottom_top( 5,  9, 10, 14),
    ggo::rect_int::from_left_right_bottom_top(10, 12, 10, 14),
    ggo::rect_int::from_left_right_bottom_top( 5,  9, 15, 15),
    ggo::rect_int::from_left_right_bottom_top(10, 12, 15, 15) });
  GGO_CHECK_EQ(tiles, expected_tiles);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(scan, scan_tiles_down_multiple_tiles)
{
  std::vector<ggo::rect_int> tiles;

  ggo::scan_tiles_down scan;
  scan(ggo::rect_int::from_left_right_bottom_top(5, 12, 10, 15), 5, 5, [&](const ggo::rect_int& tile)
  {
    tiles.push_back(tile);
  });

  const std::vector<ggo::rect_int> expected_tiles({
    ggo::rect_int::from_left_right_bottom_top( 5,  9, 11, 15),
    ggo::rect_int::from_left_right_bottom_top(10, 12, 11, 15),
    ggo::rect_int::from_left_right_bottom_top( 5,  9, 10, 10),
    ggo::rect_int::from_left_right_bottom_top(10, 12, 10, 10) });
  GGO_CHECK_EQ(tiles, expected_tiles);
}




