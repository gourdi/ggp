#ifndef __GGO_KAME_ANIM_ARTIST__
#define __GGO_KAME_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_shapes3d.h>
#include <memory>
#include <map>

namespace ggo
{
  class scene_builder;

  class kame_animation_artist : public animation_artist_abc
  {
  public:

                          kame_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void	                init() override;
    bool                  update() override;
    void                  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

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
      timed_triangle(const timed_vertex * v1, const timed_vertex * v2, const timed_vertex * v3) : _v1(v1), _v2(v2), _v3(v3) {}

      const timed_vertex * _v1;
      const timed_vertex * _v2;
      const timed_vertex * _v3;

      ggo::vec3f get_normal() const
      {
        const ggo::triangle3d_float triangle(_v1->_prv, _v2->_prv, _v3->_prv);
        return triangle.get_normal();
      }
    };

    struct glow
    {
      ggo::pos2f _pos;
      float _radius;
      float _intensity;
      float _speed;
    };

    static std::vector<std::unique_ptr<timed_triangle>> split_triangles(const std::vector<std::unique_ptr<timed_triangle>> & triangles, std::vector<std::unique_ptr<timed_vertex>> & vertices);

    void paint_glow_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, void * buffer) const;
    void paint_glow(const glow & glow, void * buffer) const;

    int                                                               _frame_index;
    std::vector<std::unique_ptr<timed_vertex>>                        _vertices;
    std::vector<std::unique_ptr<timed_triangle>>                      _triangles;
    std::map<const timed_vertex *, std::vector<const timed_vertex *>> _neighbors;
    ggo::color_32f                                                    _color;
    std::vector<glow>                                                 _foreground_glows;
    std::vector<glow>                                                 _background_glows;
  };
}

#endif
