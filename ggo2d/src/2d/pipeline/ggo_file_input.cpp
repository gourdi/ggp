#include <2d/pipeline/ggo_file_input.h>
#include <2d/io/ggo_image_io.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  file_input_node::file_input_node(std::string filename)
    :
    _filename(filename)
  {
  }

  /////////////////////////////////////////////////////////////////////
  image file_input_node::get_image() const
  {
    return load_image(_filename);
  }
}
