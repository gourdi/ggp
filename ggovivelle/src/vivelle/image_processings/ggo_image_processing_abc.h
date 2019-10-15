#ifndef __GGO_VIVELLE_IMAGE_PROCESSING_ABC__
#define __GGO_VIVELLE_IMAGE_PROCESSING_ABC__

#include <2d/ggo_image.h>

namespace ggo
{
  class parameters;
  class command;

  class image_processing_abc
  {
  public:

    virtual ggo::image process(ggo::image img, const parameters & params) const = 0;

    static std::unique_ptr<ggo::image_processing_abc> create(const std::string & cmd_name);
  };
}

#endif
