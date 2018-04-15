#include "ggo_vivelle_blit.h"
#include <ggo_pixel_buffer.h>
#include <ggo_image_io.h>
#include <ggo_blit.h>
#include <ggo_command.h>
#include <ggo_string_helpers.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  struct blit_dispatch
  {
    template <pixel_buffer_format pbf_in, pixel_buffer_format pbf_out>
    static void call(const void * input, int input_width, int input_height, int input_line_step,
                     void * output, int output_width, int output_height, int output_line_step,
                     int x, int y)
    {
      blit<pbf_in, pbf_out>(input, input_width, input_height, input_line_step,
        output, output_width, output_height, output_line_step,
        x, y);
    }
  };

  /////////////////////////////////////////////////////////////////////
  void ggo::blit(ggo::pixel_buffer & image, const parameters & params)
  {
    auto file = params["file"];
    if (!file)
    {
      throw std::runtime_error("missing 'file' parameter");
    }

    auto blit_pixels = load_image(*file);

    ggo::pos2i pos = parse_margins(params, image.width(), image.height(), blit_pixels.width(), blit_pixels.height(), "margins");

    ggo::dispatch_pbf<blit_dispatch>(blit_pixels.pbf(), image.pbf(),
      blit_pixels.data(), blit_pixels.width(), blit_pixels.height(), blit_pixels.line_byte_step(),
      image.data(), image.width(), image.height(), image.line_byte_step(),
      pos.x(), pos.y());
  }
}