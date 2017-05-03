#ifndef __GGO_BUFFER_ITERATOR__
#define __GGO_BUFFER_ITERATOR__

#include <ggo_memory.h>
#include <ggo_data_access.h>

/////////////////////////////////////////////////////////////////////
// Raw buffer iterator.
namespace ggo
{
  template <int item_byte_step, typename accessor_t, typename void_t>
  class raw_buffer_iterator
  {
  public:

    using type = typename accessor_t::type;

    raw_buffer_iterator(void_t * ptr) :
      _ptr(ptr) {
    }
    
    typename accessor_t::type read() const {
      return accessor_t::read(_ptr);
    }
    typename accessor_t::type read(const int offset) const {
      return accessor_t::read(ptr_offset(_ptr, offset * item_byte_step));
    }

    void write(const typename accessor_t::type & v) {
      accessor_t::write(_ptr, v);
    }
    void write(const typename accessor_t::type & v, const int offset) {
      accessor_t::write(ptr_offset(_ptr, offset * item_byte_step), v);
    }

    void move_nxt() {
      _ptr = ptr_offset(_ptr, item_byte_step);
    }
    void move(const int offset) {
      _ptr = ptr_offset(_ptr, offset * item_byte_step);
    }

    bool operator==(const void * ptr) { return _ptr == ptr; }
    bool operator!=(const void * ptr) { return _ptr != ptr; }

  private:

    void_t * _ptr;
  };
}

/////////////////////////////////////////////////////////////////////
// Typed buffer iterator.
namespace ggo
{
  template <typename data_t, typename accessor_t>
  class typed_buffer_iterator
  {
  public:

    using type = typename accessor_t::type;

    typed_buffer_iterator(data_t * ptr) : _ptr(ptr) {}

    typename accessor_t::type read() const {
      return accessor_t::read(_ptr);
    }
    typename accessor_t::type read(const int offset) const {
      return accessor_t::read(_ptr + offset);
    }

    void write(const typename accessor_t::type & v) {
      accessor_t::write(_ptr, v);
    }
    void write(const typename accessor_t::type & v, const int offset) {
      accessor_t::write(_ptr + offset, v);
    }

    void move_nxt() {
      _ptr++;
    }
    void move(const int offset) {
      _ptr += offset;
    }

    bool operator==(const void * ptr) { return _ptr == ptr; }
    bool operator!=(const void * ptr) { return _ptr != ptr; }

  private:

    data_t * _ptr;
  };
}

/////////////////////////////////////////////////////////////////////
// Item byte size buffer iterator.
namespace ggo
{
  template <typename data_t, typename accessor_t>
  class item_byte_size_buffer_iterator
  {
  public:

    using type = typename accessor_t::type;

    item_byte_size_buffer_iterator(data_t * ptr, int item_byte_size) : _ptr(ptr), _item_byte_size(item_byte_size) {}

    typename accessor_t::type read() const {
      return accessor_t::read(_ptr);
    }
    typename accessor_t::type read(const int offset) const {
      return accessor_t::read(ptr_offset(_ptr, offset * _item_byte_size));
    }

    void write(const typename accessor_t::type & v) {
      accessor_t::write(_ptr, v);
    }
    void write(const typename accessor_t::type & v, const int offset) {
      accessor_t::write(ptr_offset(_ptr, offset * _item_byte_size), v);
    }

    void move_nxt() {
      _ptr = ptr_offset(_ptr, _item_byte_size);
    }
    void move(const int offset) {
      _ptr = ptr_offset(_ptr, offset * _item_byte_size);
    }

    bool operator==(const void * ptr) { return _ptr == ptr; }
    bool operator!=(const void * ptr) { return _ptr != ptr; }

  private:

    data_t * _ptr;
    const int _item_byte_size;
  };
}

/////////////////////////////////////////////////////////////////////
// Factories
namespace ggo
{
  template <typename data_t>
  auto make_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<data_t, base_data_accessor<data_t>>(ptr);
  }

  template <typename data_t>
  auto make_iterator(const data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, base_data_accessor<data_t>>(ptr);
  }

  template <typename data_t>
  auto make_const_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, base_data_accessor<data_t>>(ptr);
  }

  template <typename external_data_t, typename data_t>
  auto make_cast_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<data_t, cast_data_accessor<data_t, external_data_t>>(ptr);
  }

  template <typename external_data_t, typename data_t>
  auto make_cast_iterator(const data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, cast_data_accessor<data_t, external_data_t>>(ptr);
  }

  template <typename external_data_t, typename data_t>
  auto make_cast_const_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, cast_data_accessor<data_t, external_data_t>>(ptr);
  }

  template <typename external_data_t, int bit_shift, typename data_t>
  auto make_fixed_point_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<data_t, fixed_point_data_accessor<data_t, external_data_t, bit_shift>>(ptr);
  }

  template <typename external_data_t, int bit_shift, typename data_t>
  auto make_fixed_point_iterator(const data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, fixed_point_data_accessor<data_t, external_data_t, bit_shift>>(ptr);
  }

  template <typename external_data_t, int bit_shift, typename data_t>
  auto make_fixed_point_const_iterator(data_t * ptr)
  {
    return typed_buffer_iterator<const data_t, fixed_point_data_accessor<data_t, external_data_t, bit_shift>>(ptr);
  }
}

#endif