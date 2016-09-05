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

                            image_buffer_abc(image_buffer_abc<color_t, data_t> && rhs);
                            image_buffer_abc(int width, int height, data_t * buffer, bool delete_buffer);
    virtual                ~image_buffer_abc();

            const data_t *  data() const { return _buffer; }
            data_t *        data() { return _buffer; }

            void            operator=(image_buffer_abc<color_t, data_t> && rhs);

  // No copy.
                            image_buffer_abc(const image_buffer_abc<color_t, data_t> & rhs) = delete;
            void            operator=(const image_buffer_abc<color_t, data_t> & rhs) = delete;

  protected:
    
    data_t * _buffer = nullptr;
    bool     _delete_buffer = false;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t, typename data_t>
  image_buffer_abc<color_t, data_t>::image_buffer_abc(image_buffer_abc<color_t, data_t> && rhs)
  :
  image_abc<color_t>(width, height),
  _buffer(rhs._buffer),
  _delete_buffer(rhs._delete_buffer)
  {
    rhs._buffer = nullptr;
    rhs._delete_buffer = false;
  }


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

  /////////////////////////////////////////////////////////////////////
  template <typename color_t, typename data_t>
  void image_buffer_abc<color_t, data_t>::operator=(image_buffer_abc<color_t, data_t> && rhs)
  {
    if (_delete_buffer == true)
    {
      delete[] _buffer;
    }

    _width = rhs._width;
    _height = rhs._height;
    _buffer = rhs._buffer;
    _delete_buffer = rhs._delete_buffer;

    rhs._buffer = nullptr;
    rhs._delete_buffer = false;
  }
}

#endif
