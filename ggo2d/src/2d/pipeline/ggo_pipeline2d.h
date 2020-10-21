#include <2d/ggo_image.h>
#include <memory>

namespace ggo
{
  class pipeline2d
  {
  public:

    class node
    {
    public:

      virtual image process(ggo::image img) const = 0;
    };

    class input
    {
    public:

      virtual image get_image() const = 0;
    };

    pipeline2d(std::shared_ptr<const input> input);

    void add_node(std::shared_ptr<const node> node);

    image execute() const;

  private:

    std::shared_ptr<const input> _input;
    std::vector<std::shared_ptr<const node>> _nodes;
  };
}