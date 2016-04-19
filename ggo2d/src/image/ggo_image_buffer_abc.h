#ifndef __GGO_IMAGE_BUFFER_ABC__
#define __GGO_IMAGE_BUFFER_ABC__

#include <ggo_image_abc.h>
#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// Color are stored in buffers.
namespace ggo
{
  template <typename color_type, typename data_type>
  class image_buffer_abc : public image_abc<color_type>
  {
  public:

                              image_buffer_abc(int width, int height, data_type * buffer, bool delete_buffer);
    virtual                  ~image_buffer_abc();

            const data_type * get_buffer() const { return _buffer; }
            data_type *       get_buffer() { return _buffer; }
            

  protected:
    
    data_type * _buffer = nullptr;
    bool        _delete_buffer = false;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type, typename data_type>
  image_buffer_abc<color_type, data_type>::image_buffer_abc(int width, int height, data_type * buffer, bool delete_buffer)
  :
  image_abc(width, height),
  _buffer(buffer),
  _delete_buffer(delete_buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type, typename data_type>
  image_buffer_abc<color_type, data_type>::~image_buffer_abc()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }
}

#endif
