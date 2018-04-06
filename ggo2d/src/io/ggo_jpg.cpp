#include <ggo_jpg.h>
#include <jpeglib.h>
#include <setjmp.h>

namespace
{
  struct my_error_mgr
  {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
  };

  void my_error_exit(j_common_ptr cinfo)
  {
    my_error_mgr * myerr = reinterpret_cast<my_error_mgr *>(cinfo->err);
    longjmp(myerr->setjmp_buffer, 1);
  }
}

namespace ggo
{
  bool is_jpg(const std::string & filename)
  {
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension != "jpg" && extension != "jpeg")
    {
      return false;
    }

    jpeg_decompress_struct cinfo;

    // Set up error management.
    my_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    
    if (setjmp(jerr.setjmp_buffer))
    {
      jpeg_destroy_decompress(&cinfo);
      throw std::runtime_error("failed setting up erro management");
      return false;
    }

    // Read header.
    jpeg_create_decompress(&cinfo);

    FILE * infile = fopen(filename.c_str(), "rb");
    if (infile == nullptr)
    {
      jpeg_destroy_decompress(&cinfo);
      return false;
    }

    jpeg_stdio_src(&cinfo, infile);

    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.image_width <= 0 || cinfo.image_height <= 0)
    {
      jpeg_destroy_decompress(&cinfo);
      return false;
    }

    jpeg_destroy_decompress(&cinfo);

    return true;
  }
}