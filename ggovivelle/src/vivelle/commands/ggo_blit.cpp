#include "ggo_blit.h"
#include <kernel/ggo_string_helpers.h>
#include <2d/ggo_image.h>
#include <2d/processing/ggo_blit.h>
#include <2d/io/ggo_image_io.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void blit(ggo::image & image, const parameters & params)
  {
    auto file = params.get({ "file" });
    if (!file)
    {
      throw std::runtime_error("missing 'file' parameter");
    }

    auto blit_image = load_image(*file);

    ggo::pos2_i pos = parse_margins(params, "margins", image.size(), blit_image.size());

    ggo::blit(blit_image, image, pos.x(), pos.y());
  }
}