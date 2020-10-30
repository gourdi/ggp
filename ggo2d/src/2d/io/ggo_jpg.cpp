#include <kernel/memory/ggo_array.h>
#include <2d/ggo_image.h>
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
    ggo::size s = { int(decompressor._cinfo.output_width), int(decompressor._cinfo.output_height) };
    image image(ggo::pixel_type::rgb_8u, std::make_unique<ggo::rows_memory_layout<3, vertical_direction::down>>(s, line_byte_step));

    JSAMPLE * line_ptr = reinterpret_cast<JSAMPLE *>(image.data());
    while (decompressor._cinfo.output_scanline < decompressor._cinfo.output_height)
    {
      jpeg_read_scanlines(&decompressor._cinfo, &line_ptr, 1);
      line_ptr = move_ptr(line_ptr, line_byte_step);
    }

    jpeg_finish_decompress(&decompressor._cinfo);

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_jpg(const std::string& filename, int quality, const void * buffer, ggo::pixel_type pixel_type, const memory_layout & mem_layout)
  {
    try
    {
      jpeg_compressor compressor(filename);

      // Set up the compressor.
      compressor._cinfo.image_width = mem_layout.size().width();
      compressor._cinfo.image_height = mem_layout.size().height();
      compressor._cinfo.input_components = 3;
      compressor._cinfo.in_color_space = JCS_RGB;
      jpeg_set_defaults(&compressor._cinfo);
      jpeg_set_quality(&compressor._cinfo, quality, TRUE);

      // Go!
      jpeg_start_compress(&compressor._cinfo, TRUE);

      ggo::array_8u line(3 * mem_layout.size().width());

      int y = compressor._cinfo.image_height - 1; // Top to bottom.

      while (compressor._cinfo.next_scanline < compressor._cinfo.image_height)
      {
        uint8_t * out_ptr = line.data();

        for (int x = 0; x < mem_layout.size().width(); ++x)
        {
          ggo::rgb_8u rgb = ggo::read_rgb_8u(mem_layout.at(buffer, x, y), pixel_type);

          *out_ptr++ = rgb.r();
          *out_ptr++ = rgb.g();
          *out_ptr++ = rgb.b();
        }

        uint8_t* ptr = line.data();
        jpeg_write_scanlines(&compressor._cinfo, &ptr, 1);

        --y;
      }

      jpeg_finish_compress(&compressor._cinfo);

      return true;
    }
    catch (...)
    {
      return false;
    }
  }
}
