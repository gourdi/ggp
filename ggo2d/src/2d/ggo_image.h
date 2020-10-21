#pragma once

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_memory_layouts.h>
#include <2d/ggo_pixel_type.h>
#include <optional>

namespace ggo
{
  template <pixel_type pixel_type_, typename memory_layout_t, typename void_ptr_t>
  class image_base_t final
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);
    static_assert(memory_layout_t::item_byte_size == pixel_type_traits<pixel_type_>::pixel_byte_size);

  public:
 
    using color_t = typename pixel_type_traits<pixel_type_>::color_t;

    image_base_t(memory_layout_t mem_layout)
      : _buffer(malloc(mem_layout.buffer_byte_size()))
      , _owns_buffer(true)
      , _memory_layout(mem_layout)
    {
      static_assert(std::is_same_v<void_ptr_t, void *>);
    }

    image_base_t(const ggo::size & s) : image_base_t(memory_layout_t(s))
    {
    }

    image_base_t(void_ptr_t buffer, memory_layout_t mem_layout)
      : _buffer(buffer)
      , _owns_buffer(false)
      , _memory_layout(mem_layout)
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

    size size() const { return _memory_layout.size(); }
    int width() const { return _memory_layout.size().width(); }
    int height() const { return _memory_layout.size().height(); }
    int buffer_byte_size() const { return _memory_layout.buffer_byte_size(); }
    const memory_layout_t & memory_layout() const { return _memory_layout; }
    
    static constexpr int pixel_byte_size() { return pixel_type_traits<pixel_type_>::pixel_byte_size; }
    static constexpr ggo::pixel_type pixel_type() { return pixel_type_; }

    // Move.
    image_base_t(image_base_t && image)
      : _buffer(image._buffer)
      , _owns_buffer(image._owns_buffer)
      , _memory_layout(std::move(image._memory_layout))
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
      _memory_layout = std::move(image._memory_layout);

      image._buffer = nullptr;
    }

    // No copy. Use blit API instead.
    image_base_t(const image_base_t & image) = delete;
    void operator=(const image_base_t & image) = delete;

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  pixel_ptr(int x, int y) const { return _memory_layout.at(_buffer, x, y); }
    auto          read_pixel(int x, int y) const { return pixel_type_traits<pixel_type_>::read(pixel_ptr(x, y)); }
    auto          operator()(int x, int y) const { return read_pixel(x, y); }

    // Write interface.
    template <typename dummy_t = void_ptr_t, typename = typename std::enable_if_t<std::is_same_v<dummy_t, void *>>>
    void *        data() { return _buffer; }
    template <typename dummy_t = void_ptr_t, typename = typename std::enable_if_t<std::is_same_v<dummy_t, void *>>>
    void *        pixel_ptr(int x, int y) { return _memory_layout.at(_buffer, x, y); }
    template <typename dummy_t = void_ptr_t, typename = typename std::enable_if_t<std::is_same_v<dummy_t, void *>>>
    void          write_pixel(int x, int y, const color_t & c) { pixel_type_traits<pixel_type_>::write(pixel_ptr(x, y), c); }

    // Traversing.
    template <typename func>
    void for_each_pixel(rect_int clipping, func && f) { _memory_layout.for_each(_buffer, clipping, f); }

    template <typename func>
    void for_each_pixel(rect_int clipping, func && f) const { _memory_layout.for_each(static_cast<const void *>(_buffer), clipping, f); }

    template <typename func>
    void for_each_pixel(func&& f) { _memory_layout.for_each(_buffer, ggo::rect_int::from_size(_memory_layout.size()), f); }

    template <typename func>
    void for_each_pixel(func&& f) const { _memory_layout.for_each(static_cast<const void*>(_buffer), ggo::rect_int::from_size(_memory_layout.size()), f); }

  private:

    void_ptr_t      _buffer;
    bool            _owns_buffer;
    memory_layout_t _memory_layout;
  };

  template <ggo::pixel_type pixel_type, typename memory_layout_t = bottom_up_memory_layout<pixel_type_traits<pixel_type>::pixel_byte_size>>
  using const_image_t = image_base_t<pixel_type, memory_layout_t, const void *>;

  template <ggo::pixel_type pixel_type, typename memory_layout_t = bottom_up_memory_layout<pixel_type_traits<pixel_type>::pixel_byte_size>>
  using image_t = image_base_t<pixel_type, memory_layout_t, void *>;

  using image_rgb_8u = image_base_t<pixel_type::rgb_8u, bottom_up_memory_layout<3>, void *>;
}

namespace ggo
{
  template <typename void_ptr_t>
  class image_base final
  {
    static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);

  public:

    image_base(ggo::pixel_type pixel_type, std::unique_ptr<memory_layout> mem_layout)
      : _buffer(malloc(mem_layout->buffer_byte_size()))
      , _owns_buffer(true)
      , _pixel_type(pixel_type)
      , _memory_layout(std::move(mem_layout))
    {
      static_assert(std::is_same_v<void_ptr_t, void *>);

      GGO_ASSERT_PTR(mem_layout.get());
    }

    image_base(void_ptr_t buffer, ggo::pixel_type pixel_type, std::unique_ptr<memory_layout> mem_layout)
      : _buffer(buffer)
      , _owns_buffer(false)
      , _pixel_type(pixel_type)
      , _memory_layout(mem_layout)
    {
    }

    ~image_base()
    {
      if (_owns_buffer == true)
      {
        free(_buffer);
      }
    }

    size size() const { return _memory_layout->size(); }
    int width() const { return _memory_layout->size().width(); }
    int height() const { return _memory_layout->size().height(); }
    pixel_type pixel_type() const { return _pixel_type; }
    int pixel_byte_size() const { return get_pixel_byte_size(_pixel_type); }
    const memory_layout & memory_layout() const { return *_memory_layout; }


    // Move.
    image_base(image_base && image)
      : _buffer(image._buffer)
      , _owns_buffer(image._owns_buffer)
      , _pixel_type(image._pixel_type)
      , _memory_layout(std::move(image._memory_layout))
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
      _pixel_type = image._pixel_type;
      _memory_layout = std::move(image._memory_layout);

      image._buffer = nullptr;
    }

    // No copy. Use blit API instead.
    image_base(const image_base & image) = delete;
    void operator=(const image_base & image) = delete;

    // Read interface.
    const void *  data() const { return _buffer; }
    const void *  pixel_ptr(int x, int y) const { return _memory_layout->at(_buffer, x, y); }

    // Write interface.
    template <typename dummy_t = void_ptr_t, typename = typename std::enable_if_t<std::is_same_v<dummy_t, void *>>>
    void *        data() { return _buffer; }
    template <typename dummy_t = void_ptr_t, typename = typename std::enable_if_t<std::is_same_v<dummy_t, void *>>>
    void *        pixel_ptr(int x, int y) { return _memory_layout->at(_buffer, x, y); }

  private:

    void_ptr_t                          _buffer;
    bool                                _owns_buffer;
    ggo::pixel_type                     _pixel_type;
    std::unique_ptr<ggo::memory_layout> _memory_layout;
  };

  using const_image = image_base<const void *>;
  using image = image_base<void *>;

  template <ggo::pixel_type img_pixel_type, typename void_ptr_t>
  auto read_pixel(const image_base<void_ptr_t> & img, int x, int y)
  {
    return pixel_type_traits<img_pixel_type>::read(img.pixel_ptr(x, y));
  }

  template <ggo::pixel_type img_pixel_type>
  auto write_pixel(image_base<void *> & img, int x, int y, const typename pixel_type_traits<img_pixel_type>::color_t & c)
  {
    return pixel_type_traits<img_pixel_type>::write(img.pixel_ptr(x, y), c);
  }
}

// Dispatch.
namespace ggo
{
#if 0
  template <lines_order memory_lines_order, typename functor>
  struct dispatch_image_format_functor
  {
    template <ggo::pixel_type pixel_type, typename... args>
    static auto call(args&&... a)
    {
      return functor::template call<pixel_type, memory_lines_order>(std::forward<args>(a)...);
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
#endif
}

// Image view.
namespace ggo
{
  /*
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
    if (clipping.clip(rect_int::from_size(img.size())) == false)
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
  }*/
}

namespace ggo
{
#if 0
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
#endif
}

