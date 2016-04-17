#ifndef __GGO_BMP_CODEC__
#define __GGO_BMP_CODEC__

#include <string>
#include <stdint.h>

namespace ggo
{
  bool save_bmp(const std::string & filename, const uint8_t * rgb, int width, int height);
}

#endif


