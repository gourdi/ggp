#include "ggo_image_processing_abc.h"
#include "ggo_blit_processing.h"
#include "ggo_blur_processing.h"
#include "ggo_crop_processing.h"
#include "ggo_fill_processing.h"
#include "ggo_rescale_processing.h"
#include "../ggo_command.h"

namespace ggo
{
  std::unique_ptr<ggo::image_processing_abc> image_processing_abc::create(const std::string & cmd_name)
  {
    if (cmd_name == "blit")
    {
      return std::make_unique<blit_processing>();
    }
    else if (cmd_name == "rescale")
    {
      return std::make_unique<rescale_processing>();
    }
    else if (cmd_name == "crop")
    {
      return std::make_unique<crop_processing>();
    }
    else if (cmd_name == "fill")
    {
      return std::make_unique<fill_processing>();
    }
    else if (cmd_name == "blur")
    {
      return std::make_unique<blur_processing>();
    }
    else
    {
      throw std::runtime_error("unexpected command '" + cmd_name + "'");
    }
  }
};