#ifndef __GGO_DATA_ACCESS__
#define __GGO_DATA_ACCESS__

#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// Basic data access
namespace ggo
{
  template <typename data_t>
  struct base_data_reader
  {
    static constexpr int item_byte_step = sizeof(data_t);
    using output_t = data_t;

    static data_t read(const void * ptr) {
      return *static_cast<const data_t *>(ptr);
    }
  };

  template <typename data_t>
  struct base_data_writer
  {
    static constexpr int item_byte_step = sizeof(data_t);
    using input_t = data_t;

    static void write(void * ptr, const data_t & v) {
      *static_cast<data_t *>(ptr) = v;
    }
  };
}

/////////////////////////////////////////////////////////////////////
// Cast data access
namespace ggo
{
  template <typename internal_data_t, typename external_data_t, cast_mode cast>
  struct cast_data_reader
  {
    static constexpr int item_byte_step = sizeof(internal_data_t);
    using output_t = external_data_t;

    static external_data_t read(const void * ptr) {
      const internal_data_t * ptr_internal = static_cast<const internal_data_t *>(ptr);
      return cast_to<external_data_t, cast>(*ptr_internal);
    }
  };

  template <typename internal_data_t, typename external_data_t, cast_mode cast>
  struct cast_data_writer
  {
    static constexpr int item_byte_step = sizeof(internal_data_t);
    using input_t = external_data_t;

    static void write(void * ptr, const external_data_t & v) {
      internal_data_t * ptr_internal = static_cast<internal_data_t *>(ptr);
      *ptr_internal = cast_to<internal_data_t, cast>(v);
    }
  };
}

/////////////////////////////////////////////////////////////////////
// Fixed-point data access
namespace ggo
{
  template <typename internal_data_t, typename external_data_t, int bit_shift>
  struct fixed_point_data_writer
  {
    static_assert(std::is_unsigned<internal_data_t>::value, "expecting unsigned data type");
    static_assert(std::is_unsigned<external_data_t>::value, "expecting unsigned data type");
    static_assert(sizeof(internal_data_t) < sizeof(external_data_t), "wrong types' sizes");

    static constexpr int item_byte_step = sizeof(internal_data_t);
    using input_t = external_data_t;

    static void write(void * ptr, const external_data_t & v) {
      internal_data_t * ptr_internal = static_cast<internal_data_t *>(ptr);
      *ptr_internal = static_cast<internal_data_t>(ggo::fixed_point_div<bit_shift>(v));
    }
  };
}

#endif