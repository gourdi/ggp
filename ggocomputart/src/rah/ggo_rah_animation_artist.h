#ifndef __GGO_RAH_ANIMATION_ARTIST__
#define __GGO_RAH_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <kernel/ggo_random_interpolator_abc.h>
#include <kernel/memory/ggo_array.h>
#include <functional>

namespace ggo
{
  class rah_animation_artist : public animation_artist_abc
  {
  public:

    rah_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    //////////////////////////////////////////////////////////////
    // PARTICLE
    struct particle
    {
      struct ggo_vertical_offset_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(-0.02f, 0.02f),
            dt = ggo::rand<float>(25, 50);
        }
      };

            particle(int width, int height, float focus_dist);

      float blur_radius(int min_size, float focus_dist) const;
      float disc_radius(int min_size) const;

      virtual void  update(int min_size);
              void  paint(void * buffer, int width, int height, float focus_dist) const;
              bool  is_alive(int width, int height, float focus_dist) const;

      virtual void fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const = 0;

      ggo_vertical_offset_interpolator  _vertical_offset_interpolator;
      ggo::pos2f                        _pos;
      ggo::color_8u                     _color;
      float                             _angle = 0.f;
      float                             _dist = 0.f;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 1
    struct particle1 : public particle
    {
            particle1(int width, int height, float focus_dist);

      void  update(int min_size) override;
      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;

      struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(0.5, 1),
            dt = ggo::rand<float>(10, 20);
        }
      };

      ggo::array<ggo_radius_interpolator, 1>  _radius_interpolators;
      std::shared_ptr<ggo::polygon2d_float>   _polygon;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 2
    struct particle2 : public particle
    {
            particle2(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;

      int _point_count;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 3
    struct particle3 : public particle
    {
            particle3(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;
      void  update(int min_size) override;

      struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(0.2f, 1.f),
            dt = ggo::rand<float>(10.f, 20.f);
        }
      };

      float _width;
      float _height;
      ggo_radius_interpolator _width_interpolator;
      ggo_radius_interpolator _height_interpolator;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 4
    struct particle4 : public particle
    {
            particle4(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;
    };

    struct focus_dist_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      float _near;
      float _far;

      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand<float>(_near, _near + 0.5f * (_far - _near));
        dt = ggo::rand<float>(30.f, 50.f);
      }
    };

    int                                     _frame_index;
    std::vector<std::shared_ptr<particle>>  _particles;
    focus_dist_interpolator                 _focus_dist_interpolator;
    float                                   _focus_dist;

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    void  insert_particle(std::shared_ptr<particle> particle);

    static  std::shared_ptr<particle>  create_particle(float focus_dist, int width, int height);
  };
}

#endif
