#include "ggo_blit_processing.h"
#include <kernel/ggo_string_helpers.h>
#include <2d/ggo_image.h>
#include <2d/processing/ggo_blit.h>
#include <2d/io/ggo_image_io.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  ggo::image blit_processing::process(ggo::image img, const parameters & params) const 
  {
    auto file = params.get({ "file" });
    if (!file)
    {
      throw std::runtime_error("missing 'file' parameter");
    }

    auto blit_image = load_image(*file);

    ggo::pos2_i pos = parse_margins(params, "margins", img.size(), blit_image.size());

    ggo::blit(blit_image, img, pos.x(), pos.y());

    return img;
  }
}