#ifndef __GGO_IMAGE__
#define __GGO_IMAGE__

#include <kernel/ggo_rect_int.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <2d/ggo_pixel_type.h>
#include <optional>

namespace ggo
{
  template <pixel_type pixel_type_, lines_order memory_lines_order_, typename void_ptr_t>
  class image_base_t final
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

  public:
 
    using color_t = typename pixel_type_traits<pixel_type_>::color_t;

    image_base_t(const ggo::size & s, int line_byte_step)
      : _buffer(malloc(s.height() * line_byte_step))
      , _owns_buffer(true)
      , _size(s)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * pixel_byte_size(), line_byte_step);
      static_assert(std::is_same_v<void_ptr_t, void *>);
    }

    image_base_t(const ggo::size & s)
      : image_base_t(s, pixel_byte_size() * s.width())
    {
    }

    image_base_t(void_ptr_t buffer, const ggo::size & s, int line_byte_step)
      : _buffer(buffer)
      , _owns_buffer(false)
      , _size(s)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * pixel_byte_size(), line_byte_step);
    }

    image_base_t(void_ptr_t buffer, const ggo::size & s)
      : image_base_t(buffer, s, pixel_byte_size() * s.width())
    {
    }

    ~image_base_t()
    {
      if constexpr (std::is_same_v<void_ptr_t, void *>)
      {
        if (_owns_buffer == true)
        {
          free(_buffer);
        }
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
      , _owns_buffer(image._owns_buffer)
      , _size(image._size)
      , _line_byte_step(image._line_byte_step)
    {
      image._buffer = nullptr;
    }

    void operator=(image_base_t && image)
    {
      if (_owns_buffer == true)
      {
        free(_buffer);
      }

      _buffer = image._buffer;
      _owns_buffer = image._owns_buffer;
      _size = image._size;
      _line_byte_step = image._line_byte_step;

      image._buffer = nullptr;
    }

    // No copy. Use blit API instead.
    image_base_t(const image_base_t & image) = delete;
    void operator=(const image_base_t & image) = delete;

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  line_ptr(int y) const { int dy = memory_lines_order() == lines_order::up ? y : height() - y - 1; return move_ptr(_buffer, dy * line_byte_step()); }
    const void *  pixel_ptr(int x, int y) const { return move_ptr(line_ptr(y), x * pixel_byte_size()); }
    auto          read_pixel(int x, int y) const { return pixel_type_traits<pixel_type_>::read(pixel_ptr(x, y)); }
    auto          operator()(int x, int y) const { return read_pixel(x, y); }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) { int dy = memory_lines_order() == lines_order::up ? y : height() - y - 1; return move_ptr(_buffer, dy * line_byte_step()); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) { return move_ptr(line_ptr(y), x * pixel_byte_size()); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void          write_pixel(int x, int y, const typename color_t & c) { pixel_type_traits<pixel_type_>::write(pixel_ptr(x, y), c); }

  private:

    void_ptr_t  _buffer;
    bool        _owns_buffer;
    ggo::size   _size;
    int         _line_byte_step;
  };

  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
  using const_image_t = image_base_t<pixel_type, memory_lines_order, const void *>;

  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order = ggo::lines_order::up>
  using image_t = image_base_t<pixel_type, memory_lines_order, void *>;
}

namespace ggo
{
  template <typename void_ptr_t>
  class image_base final
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

  public:

    image_base(const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int line_byte_step)
      : _buffer(malloc(s.height() * line_byte_step))
      , _owns_buffer(true)
      , _size(s)
      , _pixel_type(pixel_type)
      , _memory_lines_order(memory_lines_order)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * get_pixel_byte_size(pixel_type), line_byte_step);
      static_assert(std::is_same_v<void_ptr_t, void *>);
    }

    image_base(const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
      : image_base(s, pixel_type, memory_lines_order, get_pixel_byte_size(pixel_type) * s.width())
    {
    }

    image_base(void_ptr_t buffer, const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int line_byte_step)
      : _buffer(buffer)
      , _owns_buffer(false)
      , _size(s)
      , _pixel_type(pixel_type)
      , _memory_lines_order(memory_lines_order)
      , _line_byte_step(line_byte_step)
    {
      GGO_ASSERT_LE(s.width() * get_pixel_byte_size(pixel_type), line_byte_step);
    }

    image_base(void_ptr_t buffer, const ggo::size & s, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
      : image_base(buffer, s, pixel_type, memory_lines_order, get_pixel_byte_size(pixel_type) * s.width())
    {
    }

    ~image_base()
    {
      if (_owns_buffer == true)
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
      , _owns_buffer(image._owns_buffer)
      , _size(image._size)
      , _memory_lines_order(image._memory_lines_order)
      , _pixel_type(image._pixel_type)
      , _line_byte_step(image._line_byte_step)
    {
      image._buffer = nullptr;
    }

    void operator=(image_base && image)
    {
      if (_owns_buffer == true)
      {
        free(_buffer);
      }

      _buffer = image._buffer;
      _owns_buffer = image._owns_buffer;
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
    const void *  line_ptr(int y) const { int dy = memory_lines_order() == lines_order::up ? y : height() - y - 1; return move_ptr(_buffer, dy * line_byte_step()); }
    const void *  pixel_ptr(int x, int y) const { return move_ptr(line_ptr(y), x * pixel_byte_size()); }

    // Write interface.
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        data() { return _buffer; }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        line_ptr(int y) { int dy = memory_lines_order() == lines_order::up ? y : height() - y - 1; return move_ptr(_buffer, dy * line_byte_step()); }
    template <typename = typename std::enable_if_t<std::is_same_v<void_ptr_t, void *>>>
    void *        pixel_ptr(int x, int y) { return move_ptr(line_ptr(y), x * pixel_byte_size()); }

  private:

    void_ptr_t        _buffer;
    bool              _owns_buffer;
    ggo::size         _size;
    ggo::pixel_type   _pixel_type;
    ggo::lines_order  _memory_lines_order;
    int               _line_byte_step;
  };

  using const_image = image_base<const void *>;
  using image = image_base<void *>;
}

// Dispatch.
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

// Image view.
namespace ggo
{
  template <typename image_t>
  class generic_view
  {
  public:

    using color_t = typename image_t::color_t;

    generic_view(image_t & img, ggo::rect_int clipping) : _img(img) {}

    size size() const { return _clipping.size(); }

  private:

    rect_int  _clipping;
    image_t & _img;
  };

  template <typename image_t>
  std::optional<generic_view<image_t>> make_image_view(image_t & img, rect_int clipping)
  {
    if (clipping.clip(rect_int::from_size(_size)) == false)
    {
      return {};
    }

    return generic_view<image_t>(img, clipping);
  }

  template <pixel_type pt, lines_order lo, typename void_ptr_t>
  std::optional<image_base_t<pt, lo, const void *>> make_image_view(const image_base_t<pt, lo, void_ptr_t> & img, ggo::rect_int clipping)
  {
    if (clipping.clip(rect_int::from_size(img.size())) == false)
    {
      return {};
    }

    const void * ptr = img.pixel_ptr(clipping.left(), lo == ggo::lines_order::up ? clipping.bottom() : clipping.top());

    return image_base_t<pt, lo, const void *>(ptr, clipping.size(), img.line_byte_step());
  }

  template <pixel_type pt, lines_order lo, typename void_ptr_t>
  std::optional<image_base_t<pt, lo, void_ptr_t>> make_image_view(image_base_t<pt, lo, void_ptr_t> & img, ggo::rect_int clipping)
  {
    if (clipping.clip(rect_int::from_size(img.size())) == false)
    {
      return {};
    }

    void_ptr_t ptr = img.pixel_ptr(clipping.left(), lo == ggo::lines_order::up ? clipping.bottom() : clipping.top());

    return image_base_t<pt, lo, void_ptr_t>(ptr, clipping.size(), img.line_byte_step());
  }
}

namespace ggo
{
  template <typename image_t, typename func_t>
  void for_each_pixel(image_t & img, ggo::rect_int clipping, func_t && func)
  {
    image_t::scanner2d_t::for_each_pixel(clipping, func);
  }

  template <typename image_t, typename func_t>
  void for_each_pixel(image_t & img, func_t && func)
  {
    auto clipping = ggo::rect_int::from_size(img.size());

    for_each_pixel(img, clipping, func);
  }
}

#endif

