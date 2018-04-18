#include "ggo_z_fog.h"
#include <kernel/math/interpolation/ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  z_fog::z_fog(const ggo::color_32f & color, float z1, float z2, float far)
  :
  _color(color),
  _z_inf(std::min(z1, z2)),
  _z_sup(std::max(z1, z2)),
  _far(far)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f z_fog::process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const
  {
    if (p1.get<2>() >= _z_sup && p2.get<2>() >= _z_sup)
    {
      return color;
    }
    else if (p1.get<2>() <= _z_inf && p2.get<2>() <= _z_inf)
    {
      return color;
    }
    else
    {
      // Sort input points.
      const ggo::pos3f & p_inf = p1.get<2>() <= p2.get<2>() ? p1 : p2;
      const ggo::pos3f & p_sup = p1.get<2>() >= p2.get<2>() ? p1 : p2;
      GGO_ASSERT(p_inf.get<2>() <= p_sup.get<2>());

      if (p_inf.get<2>() >= _z_inf && p_sup.get<2>() <= _z_sup)
      {
        // Both points are in the fog.
        GGO_ASSERT(p_inf.get<2>() <= _z_sup);
        GGO_ASSERT(p_sup.get<2>() >= _z_inf);
      
        return map_color(color, p1, p2);
      }
      else
      {
        ggo::vec3f dir(p_sup - p_inf);
        dir.normalize();
          
        if (p_sup.get<2>() <= _z_sup)
        {
          // A point under the fog and the other one inside.
          GGO_ASSERT(p_inf.get<2>() <= _z_inf && p_sup.get<2>() >= _z_inf && p_sup.get<2>() <= _z_sup);
              
          float t = (_z_inf - p_inf.get<2>()) / dir.get<2>();
          ggo::pos3f p = p_inf + t * dir;
              
          GGO_ASSERT(std::abs(p.get<2>() - _z_inf) <= 0.001f);
            
          return map_color(color, p, p_sup);
        }
        else
        if (p_inf.get<2>() >= _z_inf)
        {
          // A point above the fog and the other one inside.
          GGO_ASSERT(p_sup.get<2>() >= _z_sup && p_inf.get<2>() >= _z_inf && p_inf.get<2>() <= _z_sup);
              
          float t = (_z_sup - p_inf.get<2>()) / dir.get<2>();
          ggo::pos3f p = p_inf + t * dir;
              
          GGO_ASSERT(std::abs(p.get<2>() - _z_sup) <= 0.001f);
              
          return map_color(color, p, p_inf);
        }
        else
        {
          // A point above the fog and the other under it.
          GGO_ASSERT(p_inf.get<2>() <= _z_inf && p_sup.get<2>() >= _z_sup);
              
          float t1 = (_z_inf - p_inf.get<2>()) / dir.get<2>();
          float t2 = (_z_sup - p_inf.get<2>()) / dir.get<2>();
          ggo::pos3f p1 = p_inf + t1 * dir;
          ggo::pos3f p2 = p_inf + t2 * dir;
              
          GGO_ASSERT(std::abs(p1.get<2>() -_z_inf) < 0.001f);
          GGO_ASSERT(std::abs(p2.get<2>() -_z_sup) < 0.001f);
              
          return map_color(color, p1, p2);
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f z_fog::process_light_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const
  {
    return process_segment(p1, p2, color);
  }
      
  //////////////////////////////////////////////////////////////
  ggo::color_32f z_fog::process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const
  {
    if (std::abs(ray.dir().get<2>()) < 0.001)
    {
      if (ray.pos().get<2>() > _z_inf && ray.pos().get<2>() < _z_sup)
      {
        return _color;
      }
      else
      {
        return color;
      }
    }
    else
    {
      float t_inf = (_z_inf - ray.pos().get<2>()) / ray.dir().get<2>();
      float t_sup = (_z_sup - ray.pos().get<2>()) / ray.dir().get<2>();
        
      if (t_inf <= 0 && t_sup <= 0)
      {
        return color;
      }
      else
      if (t_inf <= 0 && t_sup >= 0)
      {
        ggo::pos3f p = ray.pos() + t_sup * ray.dir();
            
        return map_color(color, p, ray.pos());
      }
      else
      if (t_inf >= 0 && t_sup <= 0)
      {
        ggo::pos3f p = ray.pos() + t_inf * ray.dir();
            
        return map_color(color, p, ray.pos());
      }
      else
      {
        GGO_ASSERT(t_inf >= 0 && t_sup >= 0);
           
        ggo::pos3f p1 = ray.pos() + t_inf * ray.dir();
        ggo::pos3f p2 = ray.pos() + t_sup * ray.dir();
            
        return map_color(color, p1, p2);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f z_fog::map_color(const ggo::color_32f & color, const ggo::pos3f & p1, const ggo::pos3f & p2) const
  {
    float dist = ggo::distance(p1, p2);

    if (dist > _far)
    {
      return _color;
    }
    else
    {
      return ggo::linear_interpolation(0.f, color, _far, _color, dist);
    }
  }
}
