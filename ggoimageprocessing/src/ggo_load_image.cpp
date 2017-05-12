#include "ggo_load_image.h"
#include <libraw.h>
#include <memory>

#undef min
#undef max

/////////////////////////////////////////////////////////////////////
ggo::pixel_buffer ggo::load_image(const char * filename)
{
  LibRaw raw_file;

  raw_file.open_file(filename);
  raw_file.unpack();
  raw_file.dcraw_process();

  constexpr ggo::pixel_buffer_format pbf = ggo::rgb_8u_yd;

  raw_file.imgdata.params.output_bps = 8 * sizeof(ggo::color_traits<ggo::pixel_buffer_format_info<pbf>::color_t>::sample_t);
  std::unique_ptr<libraw_processed_image_t, void(*)(libraw_processed_image_t*)> mem_image(raw_file.dcraw_make_mem_image(), LibRaw::dcraw_clear_mem);

  return ggo::pixel_buffer(mem_image->data, mem_image->width, mem_image->height, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size * mem_image->width, pbf);
}