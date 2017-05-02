#ifndef __GGO_DATA_ACCESS__
#define __GGO_DATA_ACCESS__

namespace ggo
{
  template <typename data_t>
  struct base_data_accessor
  {
    using type = data_t;

    // Raw buffer interface.
    static data_t read(const void * ptr) {
      return *static_cast<const data_t *>(ptr);
    }

    static void write(void * ptr, const data_t & v) {
      *static_cast<data_t *>(ptr) = v;
    }

    // Typed buffer interface.
    static data_t read(const data_t * ptr) {
      return *ptr;
    }

    static void write(data_t * ptr, const data_t & v) {
      *ptr = v;
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

#endif