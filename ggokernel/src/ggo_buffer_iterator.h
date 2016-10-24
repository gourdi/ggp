#ifndef __GGO_BUFFER_ITERATOR__
#define __GGO_BUFFER_ITERATOR__

#include <ggo_buffer_access.h>

namespace ggo
{
  template <typename data_t>
  struct data_accessor
  {
    using type = data_t;

    static data_t read(const void * ptr) {
      return *static_cast<const data_t *>(ptr);
    }

    static void write(void * ptr, const data_t & v) {
      *static_cast<data_t *>(ptr) = v;
    }
  };

  template <typename internal_data_t, typename external_data_t>
  struct cast_data_accessor
  {
    using type = external_data_t;

    static external_data_t read(const void * ptr) {
      const internal_data_t * ptr_internal = static_cast<const internal_data_t *>(ptr);
      return ggo::to<external_data_t>(*ptr_internal);
    }

    static void write(void * ptr, const external_data_t & v) {
      internal_data_t * ptr_internal = static_cast<internal_data_t *>(ptr);
      *ptr_internal = ggo::to<internal_data_t>(v);
    }
  };

  template <typename internal_data_t, typename external_data_t, int bit_shift>
  struct fixed_point_data_accessor
  {
    static_assert(std::is_unsigned<internal_data_t>::value, "expecting unsigned data type");
    static_assert(std::is_unsigned<external_data_t>::value, "expecting unsigned data type");
    static_assert(sizeof(internal_data_t) < sizeof(external_data_t), "wrong types' sizes");

    using type = external_data_t;

    static external_data_t read(const void * ptr) {
      const internal_data_t * ptr_internal = static_cast<const internal_data_t *>(ptr);
      return static_cast<external_data_t>(*ptr_internal);
    }

    static void write(void * ptr, const external_data_t & v) {
      internal_data_t * ptr_internal = static_cast<internal_data_t *>(ptr);
      *ptr_internal = static_cast<internal_data_t>(ggo::fixed_point_div<bit_shift>(v));
    }
  };
}

namespace ggo
{
  template <int item_byte_step, typename accessor_t>
  class buffer_iterator
  {
  public:

    using type = typename accessor_t::type;

    buffer_iterator(void * ptr) :
      _ptr(ptr) {
    }
    buffer_iterator(void * ptr, int dynamic_item_byte_step) :
      _ptr(ptr),
      _item_byte_step(dynamic_item_byte_step) {
      static_assert(item_byte_step == 0, "either set a static or a dynamic item byte step");
    }

    typename accessor_t::type read() const {
      return accessor_t::read(_ptr);
    }
    typename accessor_t::type read(const int offset) const {
      return accessor_t::read(ptr_offset(_ptr, offset * (item_byte_step == 0 ? _item_byte_step : item_byte_step)));
    }

    void write(const typename accessor_t::type & v) {
      accessor_t::write(_ptr, v);
    }
    void write(const typename accessor_t::type & v, const int offset) {
      accessor_t::write(ptr_offset(_ptr, offset * (item_byte_step == 0 ? _item_byte_step : item_byte_step)), v);
    }

    void move_nxt() {
      _ptr = ptr_offset(_ptr, (item_byte_step == 0 ? _item_byte_step : item_byte_step));
    }
    void move(const int offset) {
      _ptr = ptr_offset(_ptr, offset * (item_byte_step == 0 ? _item_byte_step : item_byte_step));
    }

    bool operator==(const void * ptr) { return _ptr == ptr; }
    bool operator!=(const void * ptr) { return _ptr != ptr; }

  private:

    void * _ptr;
    int _item_byte_step = 0;
  };

  template <int item_byte_step, typename accessor_t>
  class const_buffer_iterator
  {
  public:

    using type = typename accessor_t::type;

    const_buffer_iterator(const void * ptr) :
      _ptr(ptr) {
    }
    const_buffer_iterator(const void * ptr, int dynamic_item_byte_step) :
      _ptr(ptr),
      _item_byte_step(dynamic_item_byte_step) {
      static_assert(item_byte_step == 0, "either set a static or a dynamic item byte step");
    }

    typename accessor_t::type read() const {
      return accessor_t::read(_ptr);
    }
    typename accessor_t::type read(const int offset) const {
      return accessor_t::read(ptr_offset(_ptr, offset * (item_byte_step == 0 ? _item_byte_step : item_byte_step)));
    }

    void move_nxt() {
      _ptr = ptr_offset(_ptr, (item_byte_step == 0 ? _item_byte_step : item_byte_step));
    }
    void move(const int offset) {
      _ptr = ptr_offset(_ptr, offset * (item_byte_step == 0 ? _item_byte_step : item_byte_step));
    }

    bool operator==(const void * ptr) { return _ptr == ptr; }
    bool operator!=(const void * ptr) { return _ptr != ptr; }

  private:

    const void * _ptr;
    int _item_byte_step = 0;
  };
}

#endif