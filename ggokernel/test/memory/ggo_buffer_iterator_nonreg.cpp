#include <ggo_nonreg.h>
#include <ggo_buffer_iterator.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_iterator, stride_iterator_read_only)
{
  const std::vector<uint8_t> buffer{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto it = ggo::static_item_byte_step_buffer_read_only_iterator<3, ggo::base_data_reader<uint8_t>>(buffer.data());

  GGO_CHECK_EQ(it.read(), 0);
  GGO_CHECK_EQ(it.read(1), 3);
  GGO_CHECK_EQ(it.read(2), 6);

  it.move_nxt();
  GGO_CHECK_EQ(it.read(), 3);

  it.move(2);
  GGO_CHECK_EQ(it.read(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_iterator, stride_iterator_read_write)
{
  std::vector<uint8_t> buffer{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto it = ggo::static_item_byte_step_buffer_read_write_iterator<3, ggo::base_data_reader<uint8_t>, ggo::base_data_writer<uint8_t>>(buffer.data());

  GGO_CHECK_EQ(it.read(), 0);
  GGO_CHECK_EQ(it.read(1), 3);
  GGO_CHECK_EQ(it.read(2), 6);

  it.write(10);
  GGO_CHECK_EQ(buffer[0], 10);

  it.write(11, 1);
  GGO_CHECK_EQ(buffer[3], 11);

  it.write(12, 2);
  GGO_CHECK_EQ(buffer[6], 12);

  it.move_nxt();
  it.write(13);
  GGO_CHECK_EQ(buffer[3], 13);

  it.move(2);
  GGO_CHECK_EQ(it.read(), 9);
}
