#include "ggo_blit.h"
#include <kernel/ggo_string_helpers.h>
#include <2d/ggo_image.h>
#include <2d/io/ggo_image_io.h>
#include <2d/ggo_blit.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  struct blit_dispatch
  {
    template <image_format format_in, image_format format_out>
    static void call(const void * input, int input_width, int input_height, int input_line_step,
                     void * output, int output_width, int output_height, int output_line_step,
                     int x, int y)
    {
      blit<format_in, format_out>(input, input_width, input_height, input_line_step,
        output, output_width, output_height, output_line_step,
        x, y);
    }
  };

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

    ggo::dispatch_image_format<blit_dispatch>(blit_image.format(), image.format(),
      blit_image.data(), blit_image.width(), blit_image.height(), blit_image.line_byte_step(),
      image.data(), image.width(), image.height(), image.line_byte_step(),
      pos.x(), pos.y());
  }
}