#ifndef __GGO_BUFFER_ITERATOR__
#define __GGO_BUFFER_ITERATOR__

#include <ggo_memory.h>
#include <ggo_data_access.h>

/////////////////////////////////////////////////////////////////////
// Static item byte step buffer iterator.
namespace ggo
{
  template <int item_byte_step, typename data_reader_t>
  class static_item_byte_step_buffer_read_only_iterator
  {
  public:

    using type = typename data_reader_t::output_t;

    static_item_byte_step_buffer_read_only_iterator(const void * ptr) :
      _ptr(ptr) {
    }
    
    auto read() const {
      return data_reader_t::read(_ptr);
    }
    auto read(const int offset) const {
      return data_reader_t::read(ptr_offset(_ptr, offset * item_byte_step));
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

    const void * _ptr;
  };

  template <int item_byte_step, typename data_writer_t>
  class static_item_byte_step_buffer_write_only_iterator
  {
  public:

    using type = typename data_writer_t::input_t;

    static_item_byte_step_buffer_write_only_iterator(void * ptr) :
      _ptr(ptr) {
    }

    void write(const typename data_writer_t::input_t & v) {
      data_writer_t::write(_ptr, v);
    }
    void write(const typename data_writer_t::input_t & v, const int offset) {
      data_writer_t::write(ptr_offset(_ptr, offset * item_byte_step), v);
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

    void * _ptr;
  };

  template <int item_byte_step, typename data_reader_t, typename data_writer_t>
  class static_item_byte_step_buffer_read_write_iterator
  {
  public:

    static_assert(std::is_same<typename data_reader_t::output_t, typename data_writer_t::input_t>::value);

    using type = typename data_reader_t::output_t;

    static_item_byte_step_buffer_read_write_iterator(void * ptr) :
      _ptr(ptr) {
    }

    auto read() const {
      return data_reader_t::read(_ptr);
    }
    auto read(const int offset) const {
      return data_reader_t::read(ptr_offset(_ptr, offset * item_byte_step));
    }

    void write(const typename data_writer_t::input_t & v) {
      data_writer_t::write(_ptr, v);
    }
    void write(const typename data_writer_t::input_t & v, const int offset) {
      data_writer_t::write(ptr_offset(_ptr, offset * item_byte_step), v);
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

    void * _ptr;
  };
}

/////////////////////////////////////////////////////////////////////
// Dynamic item byte step buffer iterator.
namespace ggo
{
  template <typename data_reader_t>
  class dynamic_item_byte_size_buffer_read_only_iterator
  {
  public:

    using type = typename data_reader_t::output_t;

    dynamic_item_byte_size_buffer_read_only_iterator(const void * ptr, int item_byte_size) : _ptr(ptr), _item_byte_size(item_byte_size) {}

    auto read() const {
      return data_reader_t::read(_ptr);
    }
    auto read(const int offset) const {
      return data_reader_t::read(ptr_offset(_ptr, offset * _item_byte_size));
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

    const void * _ptr;
    const int _item_byte_size;
  };

  template <typename data_writer_t>
  class dynamic_item_byte_size_buffer_write_only_iterator
  {
  public:

    using type = typename data_writer_t::input_t;

    dynamic_item_byte_size_buffer_write_only_iterator(void * ptr, int item_byte_size) : _ptr(ptr), _item_byte_size(item_byte_size) {}

    void write(const typename data_writer_t::input_t & v) {
      data_writer_t::write(_ptr, v);
    }
    void write(const typename data_writer_t::input_t & v, const int offset) {
      data_writer_t::write(ptr_offset(_ptr, offset * _item_byte_size), v);
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

    void * _ptr;
    const int _item_byte_size;
  };
}

/////////////////////////////////////////////////////////////////////
// Factories
namespace ggo
{
  template <typename data_t>
  auto make_read_only_iterator(const data_t * ptr)
  {
    return static_item_byte_step_buffer_read_only_iterator<sizeof(data_t), base_data_reader<data_t>>(ptr);
  }

  template <typename data_t>
  auto make_write_only_iterator(data_t * ptr)
  {
    return static_item_byte_step_buffer_write_only_iterator<sizeof(data_t), base_data_writer<data_t >>(ptr);
  }

  template <typename data_t>
  auto make_read_write_iterator(data_t * ptr)
  {
    return static_item_byte_step_buffer_read_write_iterator<sizeof(data_t), base_data_reader<data_t>, base_data_writer<data_t>>(ptr);
  }

  template <typename external_data_t, cast_mode cast, typename data_t>
  auto make_cast_read_only_iterator(const data_t * ptr)
  {
    using data_reader = cast_data_reader<data_t, external_data_t, cast>;

    return static_item_byte_step_buffer_read_only_iterator<sizeof(data_t), data_reader>(ptr);
  }

  template <typename external_data_t, cast_mode cast, typename data_t>
  auto make_cast_write_only_iterator(data_t * ptr)
  {
    using data_writer = cast_data_writer<data_t, external_data_t, cast>;

    return static_item_byte_step_buffer_write_only_iterator<sizeof(data_t), data_writer>(ptr);
  }

  template <typename external_data_t, cast_mode read_cast, cast_mode write_cast, typename data_t>
  auto make_cast_read_write_iterator(data_t * ptr)
  {
    using data_reader = cast_data_reader<data_t, external_data_t, read_cast>;
    using data_writer = cast_data_writer<data_t, external_data_t, write_cast>;

    return static_item_byte_step_buffer_read_write_iterator<sizeof(data_t),data_reader, data_writer>(ptr);
  }

  template <typename external_data_t, int bit_shift, typename data_t>
  auto make_fixed_point_write_only_iterator(data_t * ptr)
  {
    return static_item_byte_step_buffer_write_only_iterator<sizeof(data_t), fixed_point_data_writer<data_t, external_data_t, bit_shift>>(ptr);
  }
}

#endif