#ifndef __GGO_VIVELLE_BLUR_PROCESSING__
#define __GGO_VIVELLE_BLUR_PROCESSING__

#include <vivelle/image_processings/ggo_image_processing_abc.h>

namespace ggo
{
  class blur_processing : public image_processing_abc
  {
    ggo::image process(ggo::image img, const parameters & params) const override;
  };
}

#endif
