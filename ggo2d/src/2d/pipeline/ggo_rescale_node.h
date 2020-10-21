#include <2d/pipeline/ggo_pipeline2d.h>
#include <string>

namespace ggo
{
  class rescale_node : public pipeline2d::node
  {
  public:

    enum class algo
    {
      nearest_neighbor,
      bilinear_interpolation,
      bicubic_interpolation,
      bilinear_integration
    };

    rescale_node(algo scaling_algo, ggo::size output_size);

    image process(ggo::image img) const override;

  private:

    const algo _scaling_algo;
    const ggo::size _output_size;
  };
}
