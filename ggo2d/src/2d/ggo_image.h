#ifndef __GGO_IMAGE__
#define __GGO_IMAGE__

#include <kernel/ggo_rect_int.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <2d/ggo_pixel_type.h>
#include <optional>

namespace ggo
{
  template <pixel_type pixel_type_, lines_order memory_lines_order_, typename void_ptr_t, bool owns_buffer>
  class image_base_t
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

  public:

    using color_t = typename pixel_type_traits<pixel_type_>::color_t;

    template <typename = typename std::enable_if_t<owns_buffer>>
    image_base_t(const ggo::size & s, int line_byte_step)
      : _buffer(malloc(s.height() * line_byte_step))
      , _size(s)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * pixel_byte_size(), line_byte_step);
      static_assert(std::is_same_v<void_ptr_t, void *>);
    }

    template <typename = typename std::enable_if_t<owns_buffer>>
    image_base_t(const ggo::size & s)
      : image_base_t(s, pixel_byte_size() * s.width())
    {
    }

    template <typename = typename std::enable_if_t<!owns_buffer>>
    image_base_t(void_ptr_t buffer, const ggo::size & s, int line_byte_step)
      : _buffer(buffer)
      , _size(s)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * pixel_byte_size(), line_byte_step);
    }

    template <typename = typename std::enable_if_t<!owns_buffer>>
    image_base_t(void_ptr_t buffer, const ggo::size & s)
      : image_base_t(buffer, s, pixel_byte_size() * s.width())
    {
    }

    ~image_base_t()
    {
      if constexpr (owns_buffer == true)
      {
        free(_buffer);
      }
    }

    size size() const { return _size; }
    int width() const { return _size.width(); }
    int height() const { return _size.height(); }
    int line_byte_step() const { return _line_byte_step; }
    
    static constexpr int pixel_byte_size() { return typename pixel_type_traits<pixel_type_>::pixel_byte_size; }
    static constexpr ggo::pixel_type pixel_type() { return pixel_type_; }
    static constexpr ggo::lines_order memory_lines_order() { return memory_lines_order_; }

    // Move.
    image_base_t(image_base_t && image)
      : _buffer(image._buffer)
      , _size(image._size)
      , _line_byte_step(image._line_byte_step)
    {
      image._buffer = nullptr;
    }

    void operator=(image_base_t && image)
    {
      if constexpr (owns_buffer == true)
      {
        free(_buffer);
      }

      _buffer = image._buffer;
      _size = image._size;
      _line_byte_step = image._line_byte_step;

      image._buffer = nullptr;
    }

    // No copy. Use blit API instead.
    image_base_t(const image_base_t & image) = delete;
    void operator=(const image_base_t & image) = delete;

    // Image view.
    std::optional<image_base_t<pixel_type_, memory_lines_order_, const void *, false>> create_view(ggo::rect_int clipping) const
    {
      if (clipping.clip(rect_int::from_size(_size)) == false)
      {
        return {};
      }

      const void * ptr = pixel_ptr(clipping.left(), memory_lines_order_ == ggo::lines_order::up ? clipping.bottom() : clipping.top());

      return image_base_t<pixel_type_, memory_lines_order_, const void *, false>(ptr, clipping.size(), _line_byte_step);
    }

    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    std::optional<image_base_t<pixel_type_, memory_lines_order_, void *, false>> create_view(ggo::rect_int clipping)
    {
      if (clipping.clip(rect_int::from_size(_size)) == false)
      {
        return {};
      }

      void * ptr = pixel_ptr(clipping.left(), memory_lines_order_ == ggo::lines_order::up ? clipping.bottom() : clipping.top());

      return image_base_t<pixel_type_, memory_lines_order_, void *, false>(ptr, clipping.size(), _line_byte_step);
    }

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  line_ptr(int y) const { return ggo::get_line_ptr<memory_lines_order_>(_buffer, y, _size.height(), _line_byte_step);    }
    const void *  pixel_ptr(int x, int y) const { return ggo::get_pixel_ptr<memory_lines_order_, pixel_byte_size()>(_buffer, x, y, _size.height(), _line_byte_step); }
    auto          read_pixel(int x, int y) const { return pixel_type_traits<pixel_type_>::read(pixel_ptr(x, y)); }
    auto          operator()(int x, int y) const { return read_pixel(x, y); }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) { return get_line_ptr<memory_lines_order_>(_buffer, y, height(), _line_byte_step); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) { return get_pixel_ptr<memory_lines_order_, pixel_byte_size()>(_buffer, x, y, height(), _line_byte_step); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void          write_pixel(int x, int y, const typename color_t & c) { pixel_type_traits<pixel_type_>::write(pixel_ptr(x, y), c); }

  private:

    void_ptr_t      _buffer;
    const ggo::size _size;
    const int       _line_byte_step;
  };

  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
  using const_image_view_t = image_base_t<pixel_type, memory_lines_order, const void *, false>;

  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
  using image_view_t = image_base_t<pixel_type, memory_lines_order, void *, false>;
  
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
  using image_t = image_base_t<pixel_type, memory_lines_order, void *, true>;
}

namespace ggo
{
  template <typename void_ptr_t, bool owns_buffer>
  class image_base
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

  public:

    template <typename = typename std::enable_if_t<owns_buffer>>
    image_base(const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int line_byte_step)
      : _buffer(malloc(s.height() * line_byte_step))
      , _size(s)
      , _pixel_type(pixel_type)
      , _memory_lines_order(memory_lines_order)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * get_pixel_byte_size(pixel_type), line_byte_step);
      static_assert(std::is_same_v<void_ptr_t, void *>);
    }

    template <typename = typename std::enable_if_t<owns_buffer>>
    image_base(const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
      : image_base(s, pixel_type, memory_lines_order, get_pixel_byte_size(pixel_type) * s.width())
    {
    }

    template <typename = typename std::enable_if_t<!owns_buffer>>
    image_base(void_ptr_t buffer, const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int line_byte_step)
      : _buffer(buffer)
      , _size(s)
      , _pixel_type(pixel_type)
      , _memory_lines_order(memory_lines_order)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * get_pixel_byte_size(pixel_type), line_byte_step);
    }

    template <typename = typename std::enable_if_t<!owns_buffer>>
    image_base(void_ptr_t buffer, const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
      : image_base(buffer, s, pixel_type, memory_lines_order, get_pixel_byte_size(pixel_type) * s.width())
    {
    }

    ~image_base()
    {
      if constexpr (owns_buffer == true)
      {
        free(_buffer);
      }
    }

    size size() const { return _size; }
    int width() const { return _size.width(); }
    int height() const { return _size.height(); }
    pixel_type pixel_type() const { return _pixel_type; }
    lines_order memory_lines_order() const { return _memory_lines_order; }
    int line_byte_step() const { return _line_byte_step; }
    int pixel_byte_size() const { return get_pixel_byte_size(_pixel_type); }

    // Move.
    image_base(image_base && image)
      : _buffer(image._buffer)
      , _size(image._size)
      , _memory_lines_order(image._memory_lines_order)
      , _pixel_type(image._pixel_type)
      , _line_byte_step(image._line_byte_step)
    {
      image._buffer = nullptr;
    }

    void operator=(image_base && image)
    {
      if constexpr (owns_buffer == true)
      {
        free(_buffer);
      }

      _buffer = image._buffer;
      _size = image._size;
      _pixel_type = image._pixel_type;
      _memory_lines_order = image._memory_lines_order;
      _line_byte_step = image._line_byte_step;

      image._buffer = nullptr;
    }

    // No copy. Use blit API instead.
    image_base(const image_base & image) = delete;
    void operator=(const image_base & image) = delete;

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  line_ptr(int y) const { return ggo::get_line_ptr(_buffer, y, _size.height(), _line_byte_step, _lines_order); }
    const void *  pixel_ptr(int x, int y) const { return ggo::get_pixel_ptr(_buffer, x, y, _size.height(), _line_byte_step, _memory_lines_order, pixel_byte_size()); }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) { return ggo::get_line_ptr(_buffer, y, _size.height(), _line_byte_step, _memory_lines_order); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) { return ggo::get_pixel_ptr(_buffer, x, y, _size.height(), _line_byte_step, _memory_lines_order, pixel_byte_size()); }

  private:

    void_ptr_t        _buffer;
    ggo::size         _size;
    ggo::pixel_type   _pixel_type;
    ggo::lines_order  _memory_lines_order;
    int               _line_byte_step;
  };

  using const_image_view = image_base<const void *, false>;
  using image_view = image_base<void *, false>;
  using image = image_base<void *, true>;
}

namespace ggo
{
  template <lines_order memory_lines_order, typename functor>
  struct dispatch_image_format_functor
  {
    template <ggo::pixel_type pixel_type, typename... args>
    static auto call(args&&... a)
    {
      return functor::call<pixel_type, memory_lines_order>(std::forward<args>(a)...);
    }
  };

  template <typename functor, typename... args>
  auto dispatch_image_format(ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, args&&... a)
  {
    switch (memory_lines_order)
    {
    case lines_order::up:
      return dispatch_pixel_type<dispatch_image_format_functor<lines_order::up, functor>>(pixel_type, std::forward<args>(a)...);
    case lines_order::down:
      return dispatch_pixel_type<dispatch_image_format_functor<lines_order::down, functor>>(pixel_type, std::forward<args>(a)...);
    default:
      throw std::runtime_error("invalid memory lines order");
      break;
    }
  }
}

#endif

