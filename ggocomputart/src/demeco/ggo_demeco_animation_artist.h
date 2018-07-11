#ifndef __GGO_DEMECO_ANIMATION_ARTIST__
#define __GGO_DEMECO_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <vector>
#include <memory>
#include <2d/ggo_image.h>

namespace ggo
{
  class demeco_animation_artist : public animation_artist_abc
  {
  public:

    demeco_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, bool & finished) override;

  private:

    struct demeco
    {
      demeco(const ggo::pos2f & pos, float radius, int counter) : _pos(pos), _radius(radius), _counter(counter) {}

      virtual bool render(ggo::image & image) = 0;
      ggo::pos2f _pos;
      float _radius;
      int _counter;
    };

    struct toto : public demeco
    {
      toto(const ggo::pos2f & pos, float radius, int counter) : demeco(pos, radius, counter) {}
      bool render(ggo::image & image) override;
    };

    std::vector<std::unique_ptr<demeco>> _demecos;
    ggo::image _render_image;
    ggo::image _shadow_image;
  };
}

#endif