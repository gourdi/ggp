#ifndef __GGO_KAME_ANIM_ARTIST__
#define __GGO_KAME_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <memory>

namespace ggo
{
  class scene_builder;

  class kame_animation_artist : public animation_artist_abc
  {
  public:

          kame_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void	init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    struct timed_vertex
    {
      timed_vertex(const ggo::pos3f & v) : _prv(v), _cur(v), _laplacian(v), _smoothed(v) {}
      
      ggo::pos3f _prv;
      ggo::pos3f _cur;
      ggo::pos3f _laplacian;
      ggo::pos3f _smoothed;

      void set_length(float l)
      {
        _prv.set_length(l);
        _cur.set_length(l);
        _laplacian.set_length(l);
        _smoothed.set_length(l);
      }
    };

    struct timed_triangle
    {
      timed_vertex * _v1;
      timed_vertex * _v2;
      timed_vertex * _v3;
    };

    static std::vector<timed_triangle> split_triangles(const std::vector<timed_triangle> & triangles, std::vector<std::unique_ptr<timed_vertex>> & vertices);


    int                                         _frame_index;
    std::vector<std::unique_ptr<timed_vertex>>  _vertices;
    std::vector<timed_triangle>                 _triangles;
  };
}

#endif
