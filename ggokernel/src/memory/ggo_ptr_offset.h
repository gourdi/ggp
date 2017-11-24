#ifndef __GGO_PTR_OFFSET__
#define __GGO_PTR_OFFSET__

//////////////////////////////////////////////////////////////
// Pointer offset.
namespace ggo
{
  template <typename data_t> data_t *       ptr_offset(data_t * ptr, int offset) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <typename data_t> const data_t * ptr_offset(const data_t * ptr, int offset) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }

  template <int offset, typename data_t> data_t *       ptr_offset(data_t * ptr) { return reinterpret_cast<data_t *>(reinterpret_cast<uint8_t *>(ptr) + offset); }
  template <int offset, typename data_t> const data_t * ptr_offset(const data_t * ptr) { return reinterpret_cast<const data_t *>(reinterpret_cast<const uint8_t *>(ptr) + offset); }
}

#endif
