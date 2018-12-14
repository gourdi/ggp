#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_jpg.h>
#include <jpeglib.h>
#include <setjmp.h>

static_assert(sizeof(JSAMPLE) == 1);

namespace
{
  struct error_handler
  {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
  };

  void handle_error(j_common_ptr cinfo)
  {
    error_handler * err = reinterpret_cast<error_handler *>(cinfo->err);
    longjmp(err->setjmp_buffer, 1);
  }

  struct jpeg_decompressor final
  {
    jpeg_decompressor(const std::string & filename)
    {
      _cinfo.err = jpeg_std_error(&_jerr.pub);
      _jerr.pub.error_exit = handle_error;

      if (setjmp(_jerr.setjmp_buffer))
      {
        throw std::runtime_error("failed setting up error management");
      }

      jpeg_create_decompress(&_cinfo);

      _infile = fopen(filename.c_str(), "rb");
      if (_infile == nullptr)
      {
        jpeg_destroy_decompress(&_cinfo);
        throw std::runtime_error("failed opening file");
      }

      jpeg_stdio_src(&_cinfo, _infile);
    }

    ~jpeg_decompressor()
    {
      fclose(_infile);
      jpeg_destroy_decompress(&_cinfo);
    }

    jpeg_decompress_struct _cinfo;
    error_handler _jerr;
    FILE * _infile;
  };

  struct jpeg_compressor final
  {
    jpeg_compressor(const std::string & filename)
    {
      _cinfo.err = jpeg_std_error(&_jerr.pub);
      _jerr.pub.error_exit = handle_error;

      if (setjmp(_jerr.setjmp_buffer))
      {
        throw std::runtime_error("failed setting up erro management");
      }

      jpeg_create_compress(&_cinfo);

      _outfile = fopen(filename.c_str(), "wb");
      if (_outfile == nullptr)
      {
        jpeg_destroy_compress(&_cinfo);
        throw std::runtime_error("failed opening file");
      }

      jpeg_stdio_dest(&_cinfo, _outfile);
    }

    ~jpeg_compressor()
    {
      fclose(_outfile);
      jpeg_destroy_compress(&_cinfo);
    }

    jpeg_compress_struct _cinfo;
    error_handler _jerr;
    FILE * _outfile;
  };

  //////////////////////////////////////////////////////////////
  // Write pixels (note that pixels are stored BGR from bottom to top).
  struct write_pixels
  {
    template <ggo::image_format format>
    static void call(const void * buffer, int width, int height, int line_byte_step, jpeg_compress_struct & cinfo)
    {
      using format_traits = ggo::image_format_traits<format>;

      ggo::array_8u line(3 * width);

      int y = cinfo.image_height - 1; // Top to bottom.

      while (cinfo.next_scanline < cinfo.image_height)
      {
        const void * in_ptr = ggo::get_line_ptr<format_traits::lines_order>(buffer, y, height, line_byte_step);
        uint8_t * out_ptr = line.data();

        for (int x = 0; x < width; ++x)
        {
          auto c = ggo::read_pixel<format>(in_ptr);
          ggo::rgb_8u rgb = ggo::convert_color_to<ggo::rgb_8u>(c);

          *out_ptr++ = rgb.r();
          *out_ptr++ = rgb.g();
          *out_ptr++ = rgb.b();

          in_ptr = ggo::ptr_offset<format_traits::pixel_byte_size>(in_ptr);
        }

        uint8_t * ptr = line.data();
        jpeg_write_scanlines(&cinfo, &ptr, 1);

        --y;
      }
    }
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  image load_jpg(const std::string & filename)
  {
    jpeg_decompressor decompressor(filename);

    // Header.
    jpeg_read_header(&decompressor._cinfo, TRUE);


    if (decompressor._cinfo.image_width <= 0 || decompressor._cinfo.image_height <= 0)
    {
      throw std::runtime_error("invalid image dimension");
    }

    if (decompressor._cinfo.out_color_space != JCS_RGB)
    {
      throw std::runtime_error("image is not rgb");
    }

    // Pixels.
    jpeg_start_decompress(&decompressor._cinfo);

    int line_byte_step = 3 * decompressor._cinfo.output_width;
    image image(decompressor._cinfo.output_width, decompressor._cinfo.output_height, line_byte_step, ggo::rgb_8u_yd);

    JSAMPLE * line_ptr = reinterpret_cast<JSAMPLE *>(image.data());
    while (decompressor._cinfo.output_scanline < decompressor._cinfo.output_height)
    {
      jpeg_read_scanlines(&decompressor._cinfo, &line_ptr, 1);
      line_ptr = ptr_offset(line_ptr, line_byte_step);
    }

    jpeg_finish_decompress(&decompressor._cinfo);

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_jpg(const std::string & filename, int quality, const void * buffer, image_format format, int width, int height, int line_byte_step)
  {
    try
    {
      jpeg_compressor compressor(filename);

      // Set up the compressor.
      compressor._cinfo.image_width = width;
      compressor._cinfo.image_height = height;
      compressor._cinfo.input_components = 3;
      compressor._cinfo.in_color_space = JCS_RGB;
      jpeg_set_defaults(&compressor._cinfo);
      jpeg_set_quality(&compressor._cinfo, quality, TRUE);

      // Go!
      jpeg_start_compress(&compressor._cinfo, TRUE);

      ggo::dispatch_image_format<write_pixels>(format, buffer, width, height, line_byte_step, compressor._cinfo);

      jpeg_finish_compress(&compressor._cinfo);

      return true;
    }
    catch (...)
    {
      return false;
    }
  }
}
