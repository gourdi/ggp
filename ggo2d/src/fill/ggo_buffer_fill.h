#ifndef __GGO_BUFFER_FILL__
#define __GGO_BUFFER_FILL__

// Fill with solid color.
namespace ggo
{
  template <pixel_buffer_format pbf, typename color_t = typename pixel_buffer_format_info<pbf>::color_t>
  void fill_solid(void * buffer, int width, int height, int line_step, const color_t & c);
}

//////////////////////////////////////////////////////////////
// Implementation.

// Fill with solid color.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf, typename color_t>
  void fill_solid(void * buffer, int width, int height, int line_step, const color_t & c)
  {
    process_buffer<pbf>(buffer, width, height, line_step, [&](void * ptr) { set_pixel<pbf>(ptr, c); });
  }
}


#endif
