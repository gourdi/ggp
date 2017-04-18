#ifndef _GGO_PHOTO_PROCESSOR__
#define _GGO_PHOTO_PROCESSOR__

#include <ggo_pixel_buffer.h>
#include <map>
#include <string>

namespace ggo
{
  namespace image_processing
  {
    void process(const std::map<std::string, value> & parameters, int output_width, int output_height, void * output_buffer);
  };
}

#endif
