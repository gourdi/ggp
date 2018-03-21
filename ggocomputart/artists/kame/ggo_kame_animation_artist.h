#ifndef __GGO_KAME_ANIM_ARTIST__
#define __GGO_KAME_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_shapes3d.h>
#include <ggo_random_interpolator_abc.h>
#include <ggo_link.h>
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
    bool                  prepare_frame() override;
    void                  process_frame(void * buffer, const ggo::rect_int & clipping) override;

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

    class disp_interpolator : public random_interpolator_abc<ggo::pos2f, float>
    {
      void get_random_data(ggo::pos2f & data, float & dt) override
      {
        float angle = ggo::rand<float>(0.f, 2 * ggo::pi<float>());
        float radius = ggo::rand<float>(0.f, 1.f);
        data = ggo::from_polar(angle, radius);
        dt = ggo::rand<float>(25, 50);
      }
    };

    struct kame
    {
      std::vector<std::unique_ptr<timed_vertex>>                        _vertices;
      std::vector<std::unique_ptr<timed_triangle>>                      _triangles;
      std::map<const timed_vertex *, std::vector<const timed_vertex *>> _neighbors;
      float                                                             _scale;
      float                                                             _thickness;
      ggo::pos2f                                                        _center;
      disp_interpolator                                                 _disp_interpolator;
      ggo::vec2f                                                        _disp;

      ggo::pos2f  proj(const ggo::pos2f & p) const;
      void        update();
      void        paint(void * buffer, const animation_artist_abc & artist) const;
    };

    static  std::unique_ptr<kame>                         create_kame();
    static  std::vector<std::unique_ptr<timed_triangle>>  split_triangles(const std::vector<std::unique_ptr<timed_triangle>> & triangles, std::vector<std::unique_ptr<timed_vertex>> & vertices);

    int                                 _frame_index;
    uint8_t                             _bkgd_colors[4];
    std::vector<std::unique_ptr<kame>>  _kames;
  };
}

#endif
