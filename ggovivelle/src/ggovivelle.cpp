#include <ggo_tga.h>
#include <ggo_bmp.h>

int main(int argc, char ** argv)
{
  auto bmp = ggo::load_bmp("d:\\test.bmp");
  ggo::save_tga("d:\\test2.tga", bmp.data(), bmp.pbf(), bmp.width(), bmp.height(), bmp.line_byte_step());

  auto tga = ggo::load_tga("d:\\test.tga");
  ggo::save_bmp("d:\\test2.bmp", tga.data(), tga.pbf(), tga.width(), tga.height(), tga.line_byte_step());


  //auto image = ggo::load_tga("d:\\test.bmp");

  //ggo::save_bmp("d:\\test2.bmp", image.data(), image.pbf(), image.width(), image.height(), image.line_byte_step());
}
