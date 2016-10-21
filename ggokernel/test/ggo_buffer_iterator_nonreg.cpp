#include <ggo_nonreg.h>
#include <ggo_buffer_iterator.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_iterator, read_only)
{
  const std::vector<uint8_t> buffer{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  ggo::const_buffer_iterator<3, ggo::data_accessor<uint8_t>> it(buffer.data());

  GGO_CHECK_EQ(it.read(), 0);
  GGO_CHECK_EQ(it.read(1), 3);
  GGO_CHECK_EQ(it.read(2), 6);

  it.move_nxt();
  GGO_CHECK_EQ(it.read(), 3);

  it.move(2);
  GGO_CHECK_EQ(it.read(), 9);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_iterator, read_write)
{
  std::vector<uint8_t> buffer{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  ggo::buffer_iterator<3, ggo::data_accessor<uint8_t>> it(buffer.data());

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

/////////////////////////////////////////////////////////////////////
namespace
{
  template <typename iterator_t>
  bool check_float_data_iterator(iterator_t & it, uint8_t * buffer)
  {
    it.write(10.f);
    if (std::abs(it.read() - 10.f) > 0.001f)
      return false;
    if (it != buffer)
      return false;

    it.write(11.f, 1);
    if (std::abs(it.read(1) - 11.f) > 0.001f)
      return false;

    it.move_nxt();
    it.write(12.f);
    if (std::abs(it.read() - 12.f) > 0.001f)
      return false;
    if (it != buffer + 5)
      return false;

    it.move(2);
    it.write(13.f);
    if (std::abs(it.read() - 13.f) > 0.001f)
      return false;
    if (it != buffer + 15)
      return false;

    return true;
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(buffer_iterator, float_data)
{
  std::vector<uint8_t> buffer1(4 * (sizeof(float) + 1), 0);
  std::vector<uint8_t> buffer2(4 * (sizeof(float) + 1), 0);

  ggo::buffer_iterator<5, ggo::data_accessor<float>> it1(buffer1.data());
  ggo::buffer_iterator<0, ggo::data_accessor<float>> it2(buffer2.data(), 5);

  check_float_data_iterator(it1, buffer1.data());
  check_float_data_iterator(it2, buffer2.data());
}

