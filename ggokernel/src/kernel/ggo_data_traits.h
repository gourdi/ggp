#ifndef __GGO_DATA_TRAITS__
#define __GGO_DATA_TRAITS__

namespace ggo
{
  template <typename data_type>
  struct base_data_traits
  {
    using data_t = data_type;
    using processing_t = data_type;

    static  constexpr size_t input_item_byte_size = sizeof(data_t);
    static  constexpr size_t output_item_byte_size = sizeof(data_t);

    static  data_t  read(const void * ptr) { return *static_cast<const data_t *>(ptr); }
    static  void    write(void * ptr, const data_t & v) { *static_cast<data_t *>(ptr) = v; }
    static  data_t  from_data_to_processing(const data_t & v) { return v; }
    static  data_t  from_processing_to_data(const data_t & v) { return v; }
  };
}

#endif
