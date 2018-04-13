#ifndef __GGO_VIVELLE_BLIT__
#define __GGO_VIVELLE_BLIT__

#include <map>
#include <string>

namespace ggo
{
  class pixel_buffer;

  void blit(ggo::pixel_buffer & image, std::map<std::string, std::string> & parameters);
}

#endif
