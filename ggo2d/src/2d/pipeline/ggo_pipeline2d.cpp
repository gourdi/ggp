#include <2d/pipeline/ggo_pipeline2d.h>
#include <memory>

namespace ggo
{
  pipeline2d::pipeline2d(std::shared_ptr<const input> input)
    :
    _input(input)
  {

  }

  void pipeline2d::add_node(std::shared_ptr<const node> node)
  {
    _nodes.push_back(node);
  }

  image pipeline2d::execute() const
  {
    image img = _input->get_image();

    for (const auto & node : _nodes)
    {
      img = node->process(std::move(img));
    }

    return img;
  }
}