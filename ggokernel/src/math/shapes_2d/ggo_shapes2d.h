#ifndef __GGO_SHAPES2D__
#define __GGO_SHAPES2D__

#include <vector>
#include <ggo_kernel.h>
#include <ggo_quadratic.h>
#include <ggo_set2.h>
#include <ggo_distance2d.h>
#include <ggo_linear_algebra2d.h>

//////////////////////////////////////////////////////////////////
// RECTANGLE_INTERSECTION
namespace ggo
{
  enum class rect_intersection
  {
    RECT_IN_SHAPE,
    SHAPE_IN_RECT,
    PARTIAL_OVERLAP,
    DISJOINTS,
    UNKNOWN
  };

  template <typename T>
  struct rect_data
  {
    set2<T>  _pos;
    T        _width;
    T        _height;
  };

  template <typename T>
  rect_data<T> rect_data_union(const rect_data<T> & rect1, const rect_data<T> & rect2)
  {
    T left1 = rect1._pos.x();
    T bottom1 = rect1._pos.y();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.x();
    T bottom2 = rect2._pos.y();
    T right2 = left2 + rect2._width;
    T top2 = bottom2 + rect2._height;

    T left = std::min(left1, left2);
    T bottom = std::min(bottom1, bottom2);
    T right = std::max(right1, right2);
    T top = std::max(top1, top2);

    return { { left, bottom }, right - left, top - bottom };
  }

  template <typename T>
  bool rect_data_intersection(const rect_data<T> & rect1, const rect_data<T> & rect2, rect_data<T> & result)
  {
    T left1 = rect1._pos.x();
    T bottom1 = rect1._pos.y();
    T right1 = left1 + rect1._width;
    T top1 = bottom1 + rect1._height;

    T left2 = rect2._pos.x();
    T bottom2 = rect2._pos.y();
    T right2 = left2 + rect2._width;
    T top2 = bottom2 + rect2._height;

    if (right1 < left2 || right2 < left1)
    {
      return false;
    }

    if (bottom1 > top2 || bottom2 > top1)
    {
      return false;
    }

    T left = std::max(left1, left2);
    T bottom = std::max(bottom1, bottom2);
    T right = std::min(right1, right2);
    T top = std::min(top1, top2);

    result = { { left, bottom }, right - left, top - bottom };

    return true;
  }
}

//////////////////////////////////////////////////////////////////
// GEOMETRY

namespace ggo
{
  template <typename T>
  class movable_shape2d_abc
  {
  public:

    virtual	void	move(T dx, T dy) = 0;
            void	move(const ggo::set2<T> & m) { move(m.x(), m.y()); }
  };

  template <typename T>
  class rotatable_shape2d_abc
  {
  public:

    virtual	void  rotate(T angle, const ggo::set2<T> & center) = 0;
  };

  template <typename T> 
  class distancable_shape2d_abc
  {
  public:

    virtual	T dist_to_point(T x, T y) const = 0;
            T	dist_to_point(const ggo::set2<T> & p) const { return dist_to_point(p.x(), p.y()); }
  };
}

//////////////////////////////////////////////////////////////////
// PAINT

namespace ggo
{
  template <typename T>
  class samplable_shape2d_abc
  {
  public:

    virtual bool  is_point_inside(T x, T y) const = 0;
            bool  is_point_inside(const ggo::set2<T> & p) const { return is_point_inside(p.x(), p.y()); }
  };

  template <typename T>
  class bounded_shape2d_abc
  {
  public:

    virtual rect_data<T> get_bounding_rect() const = 0;
  };

  template <typename T>
  class rect_intersect_shape2d_abc
  {
  public:

    virtual	rect_intersection	get_rect_intersection(const rect_data<T> & rect_data) const = 0;
  };

  template <typename T>
  class paintable_shape2d_abc : public samplable_shape2d_abc<T>, public bounded_shape2d_abc<T>, public rect_intersect_shape2d_abc<T>
  {
  public:
    
    using samplable_shape2d_abc<T>::is_point_inside;

    std::vector<ggo::set2<T>> uniform_sampling(T delta_sample) const
    {
      std::vector<ggo::set2<T>> samples;

      auto rect_data = this->get_bounding_rect(); 
      T width = rect_data._width;
      T height = rect_data._height;
      ggo::set2<T> center(rect_data._pos.x() + width / 2, rect_data._pos.y() + height / 2);
    
      for (T y = delta_sample / 2; y < height / 2; y += delta_sample)
      {
        for (T x = delta_sample / 2; x < width / 2; x += delta_sample)
        {
          T x1 = center.x() - x;
          T x2 = center.x() + x;
          T y1 = center.y() - y;
          T y2 = center.y() + y;

          if (is_point_inside(x1, y1) == true)
          {
            samples.push_back(ggo::set2<T>(x1, y1));
          }
          if (is_point_inside(x1, y2) == true)
          {
            samples.push_back(ggo::set2<T>(x1, y2));
          }
          if (is_point_inside(x2, y1) == true)
          {
            samples.push_back(ggo::set2<T>(x2, y1));
          }
          if (is_point_inside(x2, y2) == true)
          {
            samples.push_back(ggo::set2<T>(x2, y2));
          }
        }
      }

      return samples;
    }

    std::vector<ggo::set2<T>> minimum_uniform_sampling(int samples_count, int iterations_count = 100) const
    {
      auto rect_data = this->get_bounding_rect();
      T width = rect_data._width;
      T height = rect_data._height;
      ggo::set2<T> center(rect_data._pos.x() + width / 2, rect_data._pos.y() + height / 2);
      
      if (samples_count == 1)
      {
        std::vector<ggo::set2<T>> samples;
        samples.emplace_back(center);
        return samples;
      }

      // Get the inf limit.
      T delta_inf = std::max(width, height);
      auto samples_inf = uniform_sampling(delta_inf);

      // Decrease delta_sup until we get more samples than required.
      T delta_sup = std::max(width, height);
      auto samples_sup = uniform_sampling(delta_sup);
      while (static_cast<int>(samples_sup.size()) < samples_count)
      {
        delta_sup /= 2;
        samples_sup = uniform_sampling(delta_sup);
      }

      GGO_ASSERT(samples_inf.size() <= samples_count);
      GGO_ASSERT(samples_sup.size() >= samples_count);

      // Then 'dichotomize'.
      for (int i = 0; i < iterations_count; ++i)
      {
        GGO_ASSERT(samples_inf.size() <= samples_sup.size());

        T delta_middle = (delta_inf + delta_sup) / 2;

        auto samples_middle = uniform_sampling(delta_middle);

        if (samples_count > static_cast<int>(samples_middle.size()))
        {
          delta_inf = delta_middle;
          samples_inf = samples_middle;
        }
        else
        {
          if (samples_count < static_cast<int>(samples_middle.size()))
          {
            delta_sup = delta_middle;
            samples_sup = samples_middle;
          }
          else
          {
            GGO_ASSERT(samples_middle.size() == samples_count);
            return samples_middle;
          }
        }
      }

      return samples_sup;
    }
  };

  template <typename T>
  class seed_paintable_shape2d_abc : public ggo::samplable_shape2d_abc<T>
  {
  public:

    virtual	rect_data<T> get_seed_rect() const = 0;
  };
}

#include <ggo_rect.h>
#include <ggo_segment.h>
#include <ggo_circle.h>
#include <ggo_disc.h>
#include <ggo_extended_segment.h>
#include <ggo_spline.h>
#include <ggo_polygon.h>
#include <ggo_triangle2d.h>
#include <ggo_multi_shape.h>

#include <ggo_rect.imp.h>
#include <ggo_segment.imp.h>
#include <ggo_circle.imp.h>
#include <ggo_disc.imp.h>
#include <ggo_extended_segment.imp.h>
#include <ggo_spline.imp.h>
#include <ggo_polygon.imp.h>
#include <ggo_triangle2d.imp.h>
#include <ggo_multi_shape.imp.h>

#endif
