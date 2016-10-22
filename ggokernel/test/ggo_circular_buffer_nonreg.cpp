#include <ggo_nonreg.h>
#include <ggo_circular_buffer.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(circular_buffer, test)
{
  ggo::circular_buffer<int> cb(5);
  cb.push(1);
  cb.push(2);
  cb.push(3);
  cb.push(4);
  GGO_CHECK_EQ(cb.pop(), 1);
  cb.push(5);
  cb.push(6);

  GGO_CHECK_EQ(cb.data()[0], 6);
  GGO_CHECK_EQ(cb.data()[1], 2);
  GGO_CHECK_EQ(cb.data()[2], 3);
  GGO_CHECK_EQ(cb.data()[3], 4);
  GGO_CHECK_EQ(cb.data()[4], 5);

  GGO_CHECK_EQ(cb.pop(), 2);
  GGO_CHECK_EQ(cb.pop(), 3);
  GGO_CHECK_EQ(cb.pop(), 4);
  GGO_CHECK_EQ(cb.pop(), 5);
  GGO_CHECK_EQ(cb.pop(), 6);
}
