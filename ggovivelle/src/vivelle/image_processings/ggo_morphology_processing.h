#ifndef __GGO_VIVELLE_MORPHOLOGY_PROCESSING__
#define __GGO_VIVELLE_MORPHOLOGY_PROCESSING__

#include <vivelle/image_processings/ggo_image_processing_abc.h>

namespace ggo
{
  class morphology_processing : public image_processing_abc
  {
  public:

    enum class type
    {
      erosion,
      dilatation
    };

    morphology_processing(type t) : _type(t) {}

  private:

    ggo::image process(ggo::image img, const parameters & params) const override;

    ggo::image apply_rectangle_operation(ggo::image img, const std::string & params) const;
    ggo::image apply_disc_operation(ggo::image img, float radius) const;

  private:

    type _type;
  };
}

#endif
