#ifndef __GGO_IMAGE__
#define __GGO_IMAGE__

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_size.h>
#include <2d/ggo_image_format.h>
#include <optional>

namespace ggo
{
  class image_data
  {
  public:

    image_data(ggo::size s, int line_byte_step) : _size(s), _line_byte_step(line_byte_step) {}

    int       min_size() const { return std::min(_size.width(), _size.height()); }
    ggo::size size() const { return _size; }
    int       width() const { return _size.width(); }
    int       height() const { return _size.height(); }
    int       line_byte_step() const { return _line_byte_step; }

    template <typename func_t>
    void      for_each_pixel(func_t f) const
    {
      int w = _size.width();
      int h = _size.width();

      for (int y = 0; y < h; ++y)
      {
        for (int x = 0; x < w; ++x)
        {
          f(x, y);
        }
      }
    }

  protected:

    ggo::size _size;
    int       _line_byte_step;
  };

  class image_data_format : public image_data
  {
  public:

    image_data_format(ggo::size s, int line_byte_step, image_format format) : image_data(s, line_byte_step), _format(format) {}

    image_format format() const { return _format; }

  protected:

    image_format _format;
  };
}

namespace ggo
{
  template <image_format img_format, typename void_ptr_t>
  class image_wrapper_base_t : public image_data
  {
  public:

    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

    using format_traits = typename image_format_traits<img_format>;
    using color_t = typename image_format_traits<img_format>::color_t;

    image_wrapper_base_t(void_ptr_t buffer, ggo::size s, int line_byte_step) : image_data(s, line_byte_step), _buffer(buffer) {}

    constexpr image_format  format() const { return img_format; }

    // Image view.
    std::optional<image_wrapper_base_t<img_format, void_ptr_t>> create_view(ggo::rect_int clipping) const
    {
      if (clipping.clip(rect_int::from_size(_size)) == false)
      {
        return {};
      }

      void_ptr_t ptr = nullptr;
      if constexpr (format_traits::lines_order == ggo::memory_lines_order::bottom_up)
      {
        ptr = get_pixel_ptr<img_format>(_buffer, clipping.left(), clipping.bottom(), _size.height(), _line_byte_step);
      }
      else
      {
        ptr = get_pixel_ptr<img_format>(_buffer, clipping.left(), clipping.top(), _size.height(), _line_byte_step);
      }
      return image_wrapper_base_t<img_format, void_ptr_t>(ptr, clipping.size(), _line_byte_step);
    }

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  line_ptr(int y) const { return get_line_ptr<format_traits::lines_order>(_buffer, y, height(), _line_byte_step); }
    const void *  pixel_ptr(int x, int y) const { return get_pixel_ptr<format>(_buffer, x, y, height(), _line_byte_step); }

    auto          read_pixel(int x, int y) const
    {
      return ggo::read_pixel<img_format>(_buffer, x, y, _size.height(), _line_byte_step);
    }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) { return get_line_ptr<format_traits::lines_order>(_buffer, y, height(), _line_byte_step); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) { return get_pixel_ptr<format>(_buffer, x, y, height(), _line_byte_step); }

    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void          fill(const typename color_t & c)
    {
      void * ptr = _buffer;
      int w = _size.width();
      int h = _size.height();

      for (int y = 0; y < h; ++y)
      {
        void * line_ptr = ptr;
        void * end = move_ptr(ptr, w * image_format_traits<img_format>::pixel_byte_size);

        for (; line_ptr != end; line_ptr = move_ptr<image_format_traits<img_format>::pixel_byte_size>(line_ptr))
        {
          ggo::write_pixel<img_format>(line_ptr, c);
        }

        ptr = move_ptr(ptr, _line_byte_step);
      }
    }

    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void          write_pixel(int x, int y, const typename color_t & c)
    {
      ggo::write_pixel<img_format>(_buffer, x, y, _size.height(), _line_byte_step, c);
    }

  protected:

    void_ptr_t _buffer;
  };

  template <image_format img_format>
  using const_image_wrapper_t = image_wrapper_base_t<img_format, const void *>;

  template <image_format img_format>
  using image_wrapper_t = image_wrapper_base_t<img_format, void *>;

  template <image_format img_format>
  class image_t final : public image_wrapper_t<img_format>
  {
  public:

    image_t(ggo::size s) : image_wrapper_t<img_format>(malloc(s.height() * s.width() * format_traits::pixel_byte_size), s, s.width() * format_traits::pixel_byte_size) {}
    ~image_t() { free(_buffer); }

    image_t(image_t && img);
    void operator=(image_t && img);
  };
}

namespace ggo
{
  template <typename void_ptr_t>
  class image_wrapper_base : public image_data_format
  {
  public:

    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

    image_wrapper_base(void_ptr_t buffer, ggo::size s, int line_byte_step, image_format format) :
      image_data_format(s, line_byte_step, format),
      _buffer(buffer)
    {

    }

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  line_ptr(int y) const {
      return get_line_ptr(_buffer, y, _size.height(), _line_byte_step, _format);
    }
    const void *  pixel_ptr(int x, int y) const {
      return get_pixel_ptr(_buffer, x, y, _size.height(), _line_byte_step, _format);
    }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) {
      return get_line_ptr(_buffer, y, _size.height(), _line_byte_step, _format);
    }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) {
      return get_pixel_ptr(_buffer, x, y, _size.height(), _line_byte_step, _format);
    }

  protected:

    void_ptr_t _buffer;
  };

  using const_image_wrapper = image_wrapper_base<const void *>;
  using image_wrapper = image_wrapper_base<void *>;

  class image final : public image_wrapper
  {
  public:

    image(ggo::size s, image_format format);
    image(ggo::size s, int line_byte_step, image_format format);
    ~image();

    image(image && img);
    void operator=(image && img);
  };
}

#include <2d/ggo_image.imp.h>

#endif

