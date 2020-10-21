#include <2d/pipeline/ggo_pipeline2d.h>
#include <string>

namespace ggo
{
  class file_input_node : public pipeline2d::input
  {
  public:

    file_input_node(std::string filename);

    image get_image() const override;

  private:

    const std::string _filename;
  };
}
