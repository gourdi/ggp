#ifndef __GGO_DUPECHECK_ANIMATION_ARTIST__
#define __GGO_DUPECHECK_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>

namespace ggo
{
  class dupecheck_animation_artist : public animation_artist_abc
  {
  public:

                  dupecheck_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void          init() override;
    bool          update() override;
    void          render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    ggo::pos2f	  get_position(int frame_index);
    ggo::color_8u get_color(int frame_index);

  private:

    class dupecheck_animate_abc : public ggo::path_animate_abc
    {
    protected:

      float		      _opacity;
      ggo::color_8u	_color;
      float		      _radius;
      float		      _dradius;

                    dupecheck_animate_abc(ggo::pos2f pos, ggo::path_abc * path, const ggo::color_8u & color, int render_min_size);

              bool	update(int frame_index, const ggo::pos2f & pos) override;
      virtual void  update() = 0;
    };

    class animate1 : public dupecheck_animate_abc
    {
    public:

            animate1(const ggo::pos2f & pos, ggo::path_abc * path, const ggo::color_8u & color, int render_min_size);
      void  update() override {};
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    private:

      float	_width;
      float	_distance;
      int	  _count;
    };

    class animate2 : public dupecheck_animate_abc
    {
    public:

            animate2(const ggo::pos2f & pos, ggo::path_abc * path, const ggo::color_8u & color, int render_min_size);
      void  update() override {};
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    private:

      float	_width;
    };

    class animate3 : public dupecheck_animate_abc
    {
    public:

            animate3(const ggo::pos2f & pos, ggo::path_abc * path, const ggo::color_8u & color, int render_min_size);
      void  update() override;
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    private:

      int		_shapes_count;
      int		_vertices_count;
      float	_angle;
      float	_dangle;
      float	_angle_shape;
      float	_dangle_shape;
      float	_shape_size;
    };

    struct background_color
    {
      float _y;
      float	_val;
      float	_radius;
      float	_angle;
      float	_dangle;
      float	_var;
    };

    int                           _frame_index;
    std::vector<background_color> _bkgd_colors;
    ggo::color_32f 					      _colors[4];
    ggo::pos2f 					          _points[4];
    ggo::animator						      _animator;
  };
}

#endif

