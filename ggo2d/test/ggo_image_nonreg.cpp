#include "ggo_2d_nonreg.h"
#include <2d/ggo_image.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, move)
{
  ggo::image img1({ 20, 30 }, ggo::pixel_type::rgb_8u, ggo::lines_order::up);
  ggo::image img2({ 10, 20 }, ggo::pixel_type::rgb_8u, ggo::lines_order::down);

  void * ptr = img2.data();

  img1 = std::move(img2);

  GGO_CHECK_EQ(img1.data(), ptr);
  GGO_CHECK_EQ(img1.size(), ggo::size(10, 20));
  GGO_CHECK_EQ(img1.pixel_type(), ggo::pixel_type::rgb_8u);
  GGO_CHECK_EQ(img1.memory_lines_order(), ggo::lines_order::down);
  GGO_CHECK_EQ(img1.line_byte_step(), 30);
  GGO_CHECK_EQ(img2.data(), nullptr);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, image_view_yd)
{
  auto img = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 00, 01, 02, 03, 04, 05 },
    { 10, 11, 12, 13, 14, 15 },
    { 20, 21, 22, 23, 24, 25 },
    { 30, 31, 32, 33, 34, 35 },
    { 40, 41, 42, 43, 44, 45 } });

  auto view = img.create_view(ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 2));

  GGO_CHECK_EQ(view->read_pixel(0, 0), 32);
  GGO_CHECK_EQ(view->read_pixel(1, 0), 33);
  GGO_CHECK_EQ(view->read_pixel(2, 0), 34);
  GGO_CHECK_EQ(view->read_pixel(0, 1), 22);
  GGO_CHECK_EQ(view->read_pixel(1, 1), 23);
  GGO_CHECK_EQ(view->read_pixel(2, 1), 24);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, image_view_yu)
{
  auto img = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 00, 01, 02, 03, 04, 05 },
    { 10, 11, 12, 13, 14, 15 },
    { 20, 21, 22, 23, 24, 25 },
    { 30, 31, 32, 33, 34, 35 },
    { 40, 41, 42, 43, 44, 45 } });

  auto view = img.create_view(ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 2));

  GGO_CHECK_EQ(view->read_pixel(0, 0), 12);
  GGO_CHECK_EQ(view->read_pixel(1, 0), 13);
  GGO_CHECK_EQ(view->read_pixel(2, 0), 14);
  GGO_CHECK_EQ(view->read_pixel(0, 1), 22);
  GGO_CHECK_EQ(view->read_pixel(1, 1), 23);
  GGO_CHECK_EQ(view->read_pixel(2, 1), 24);
}




namespace ggo
{
  template <ggo::pixel_type pixel_type>
  struct image_input_iterator_1d
  {
    using data_t = typename pixel_type_traits<pixel_type>::color_t;

    image_input_iterator_1d(const void * ptr, std::ptrdiff_t pixel_offset) : _ptr(ptr), _pixel_offset(pixel_offset) {}

    const void * _ptr;
    const std::ptrdiff_t _pixel_offset;

    void operator++() { _ptr = ggo::move_ptr(_ptr, _pixel_offset); }
    void operator--() { _ptr = ggo::move_ptr(_ptr, -_pixel_offset); }
    image_input_iterator_1d operator+(std::ptrdiff_t delta) { return image_input_iterator_1d(ggo::move_ptr(_ptr, _pixel_offset * delta)); }
    image_input_iterator_1d operator-(std::ptrdiff_t delta) { return image_input_iterator_1d(ggo::move_ptr(_ptr, -_pixel_offset * delta)); }
    bool operator==(const image_input_iterator_1d & it) { return _ptr == it._ptr; }
    bool operator!=(const image_input_iterator_1d & it) { return _ptr != it._ptr; }
    data_t operator()(std::ptrdiff_t delta) const { return  pixel_type_traits<pixel_type>::read(ggo::move_ptr(_ptr, _pixel_offset * delta)); }
  };

  template <pixel_type pixel_type, lines_order memory_lines_order, typename void_ptr_t, bool owns_buffer>
  auto make_image_input_line_iterator_1d(const image_base_t<pixel_type, memory_lines_order, void_ptr_t, owns_buffer> & image, int y)
  {
    return image_input_iterator_1d(ggo::move_ptr(image.data(), y * image.line_byte_step(), image.pixel_byte_size()));
  }

  template <typename void_ptr_t, bool owns_buffer>
  auto make_image_input_line_iterator_1d(const image_base<void_ptr_t, owns_buffer> & image, int y)
  {
    return image_input_iterator_1d(ggo::move_ptr(image.data(), y * image.line_byte_step(), image.pixel_byte_size()));
  }

  template <pixel_type pixel_type, lines_order memory_lines_order, typename void_ptr_t, bool owns_buffer>
  auto make_image_input_column_iterator_1d(const image_base_t<pixel_type, memory_lines_order, void_ptr_t, owns_buffer> & image, int x)
  {
    return image_input_iterator_1d(ggo::move_ptr(image.data(), x * image.pixel_byte_size(), image.line_byte_step()));
  }

  template <typename void_ptr_t, bool owns_buffer>
  auto make_image_input_column_iterator_1d(const image_base<void_ptr_t, owns_buffer> & image, int x)
  {
    return image_input_iterator_1d(ggo::move_ptr(image.data(), x * image.pixel_byte_size(), image.line_byte_step()));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(signal_processing_1d, toto)
{

}






