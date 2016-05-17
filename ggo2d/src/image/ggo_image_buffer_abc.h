#ifndef __GGO_IMAGE_BUFFER_ABC__
#define __GGO_IMAGE_BUFFER_ABC__

#include <ggo_image_abc.h>
#include <ggo_kernel.h>

/////////////////////////////////////////////////////////////////////
// Color are stored in buffers.
namespace ggo
{
  template <typename color_t, typename data_t>
  class image_buffer_abc : public image_abc<color_t>
  {
  public:

                           image_buffer_abc(int width, int height, data_t * buffer, bool delete_buffer);
    virtual               ~image_buffer_abc();

            const data_t * data() const { return _buffer; }
            data_t *       data() { return _buffer; }

  protected:
    
    data_t * _buffer = nullptr;
    bool     _delete_buffer = false;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t, typename data_t>
  image_buffer_abc<color_t, data_t>::image_buffer_abc(int width, int height, data_t * buffer, bool delete_buffer)
  :
  image_abc<color_t>(width, height),
  _buffer(buffer),
  _delete_buffer(delete_buffer)
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, typename data_t>
  image_buffer_abc<color_t, data_t>::~image_buffer_abc()
  {
    if (_delete_buffer)
    {
      delete[] _buffer;
    }
  }
}

#endif
