#ifndef __GGO_ITERATOR__
#define __GGO_ITERATOR__

#include <kernel/ggo_assert.h>

namespace ggo
{
  template <typename subtype_t, typename ptr_t, ptrdiff_t offset> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct iterator_t
  {
    iterator_t(ptr_t ptr) : _ptr(ptr) {}

    ptr_t _ptr;

    void operator++() { _ptr += offset; }
    void operator--() { _ptr -= offset; }
    bool operator==(const iterator_t & it) { return _ptr == it._ptr; }
    bool operator!=(const iterator_t & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(_ptr + offset * delta); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(_ptr - offset * delta); }
  };

  template <typename data_t_, ptrdiff_t offset = 1>
  struct input_iterator_t final : public ggo::iterator_t<input_iterator_t<data_t_, offset>, const data_t_ *, offset>
  {
    using data_t = typename data_t_;

    input_iterator_t(const data_t * ptr) : iterator_t(ptr) {}

    const data_t & get(std::ptrdiff_t delta) const { return _ptr[offset * delta]; }
    const data_t & operator*() const { return *_ptr; }
  };

  template <typename data_t_, ptrdiff_t offset = 1>
  struct output_iterator_t final : public ggo::iterator_t<output_iterator_t<data_t_, offset>, data_t_ *, offset>
  {
    using data_t = typename data_t_;

    output_iterator_t(data_t * ptr) : iterator_t(ptr) {}

    void set(const data_t & v) { *_ptr = v; }
    output_iterator_t & operator<<(const data_t & v) { *_ptr = v; _ptr += offset; return *this; }
  };

  template <typename subtype_t, typename ptr_t, ptrdiff_t offset>
  auto distance(const iterator_t<subtype_t, ptr_t, offset> & it1, const iterator_t<subtype_t, ptr_t, offset> & it2)
  {
    return (it2._ptr - it1._ptr) / offset;
  }
}

// Void * static iterator.
namespace ggo
{
  template <typename subtype_t, std::ptrdiff_t byte_offset, typename void_ptr_t> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct void_iterator_t
  {
    void_iterator_t(void_ptr_t ptr) : _ptr(ptr) {}

    void_ptr_t _ptr;

    void operator++() { _ptr = ggo::move_ptr(_ptr, byte_offset); }
    void operator--() { _ptr = ggo::move_ptr(_ptr, -byte_offset); }
    bool operator==(const void_iterator_t & it) { return _ptr == it._ptr; }
    bool operator!=(const void_iterator_t & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr,  byte_offset * delta)); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr, -byte_offset * delta)); }
  };

  template <typename subtype_t, std::ptrdiff_t byte_offset, typename void_ptr_t>
  auto distance(const void_iterator_t<subtype_t, byte_offset, void_ptr_t> & it1, const void_iterator_t<subtype_t, byte_offset, void_ptr_t> & it2)
  {
    return (static_cast<const char *>(it2._ptr) - static_cast<const char *>(it1._ptr)) / byte_offset;
  }
}

// Dynamic iterator.
namespace ggo
{
  template <typename subtype_t, typename ptr_t> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct iterator
  {
    iterator(ptr_t ptr, std::ptrdiff_t offset) : _ptr(ptr), _offset(offset) {}

    ptr_t _ptr;
    const std::ptrdiff_t _offset;

    void operator++() { _ptr += _offset; }
    void operator--() { _ptr -= _offset; }
    bool operator==(const iterator & it) { return _ptr == it._ptr; }
    bool operator!=(const iterator & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(_ptr + _offset * delta, _offset); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(_ptr - _offset * delta, _offset); }
  };

  template <typename data_t_>
  struct input_iterator final : public ggo::iterator<input_iterator<data_t_>, const data_t_ *>
  {
    using data_t = typename data_t_;

    input_iterator(const data_t * ptr, std::ptrdiff_t offset) : iterator(ptr, offset) {}

    const data_t & get(std::ptrdiff_t delta) const { return _ptr[_offset * delta]; }
    const data_t & operator*() const { return *_ptr; }
  };

  template <typename data_t_>
  struct output_iterator final : public ggo::iterator<output_iterator<data_t_>, data_t_ *>
  {
    using data_t = typename data_t_;

    output_iterator(data_t * ptr, std::ptrdiff_t offset) : iterator(ptr, offset) {}

    void set(const data_t & v) { *_ptr = v; }
    output_iterator & operator<<(const data_t & v) { *_ptr = v; _ptr += _offset; return *this; }
  };

  template <typename subtype_t, typename ptr_t>
  auto distance(const iterator<subtype_t, ptr_t> & it1, const iterator<subtype_t, ptr_t> & it2)
  {
    GGO_ASSERT_EQ(it1._offset, it2._offset);

    return (it2._ptr - it1._ptr) / it1._offset;
  }
}

// Void * dynamic iterator.
namespace ggo
{
  template <typename subtype_t, typename void_ptr_t> // <= curiously recurring template pattern, for easy + and - operators returning subclass types.
  struct void_iterator
  {
    void_iterator(void_ptr_t ptr, std::ptrdiff_t byte_offset) : _ptr(ptr), _byte_offset(byte_offset) {}

    void_ptr_t _ptr;
    const std::ptrdiff_t _byte_offset;

    void operator++() { _ptr = ggo::move_ptr(_ptr, _byte_offset); }
    void operator--() { _ptr = ggo::move_ptr(_ptr, -_byte_offset); }
    bool operator==(const void_iterator & it) { return _ptr == it._ptr; }
    bool operator!=(const void_iterator & it) { return _ptr != it._ptr; }

    subtype_t operator+(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr, _byte_offset * delta), _byte_offset); }
    subtype_t operator-(std::ptrdiff_t delta) const { return subtype_t(ggo::move_ptr(_ptr, -_byte_offset * delta), _byte_offset); }
  };

  template <typename subtype_t, typename void_ptr_t>
  auto distance(const void_iterator<subtype_t, void_ptr_t> & it1, const void_iterator<subtype_t, void_ptr_t> & it2)
  {
    GGO_ASSERT_EQ(it1._byte_offset, it1._byte_offset);
    return (static_cast<const char *>(it2._ptr) - static_cast<const char *>(it1._ptr)) / it1._byte_offset;
  }
}

#endif

