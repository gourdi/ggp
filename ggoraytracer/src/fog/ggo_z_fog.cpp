#include "ggo_z_fog.h"
#include <ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  z_fog::z_fog(const ggo::color & color, float z1, float z2, float far)
  :
  _color(color),
  _z_inf(std::min(z1, z2)),
  _z_sup(std::max(z1, z2)),
  _far(far)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color z_fog::process_segment(const ggo::point3d_float & p1, const ggo::point3d_float & p2, const ggo::color & color) const
  {
    if (p1.z() >= _z_sup && p2.z() >= _z_sup)
    {
      return color;
    }
    else if (p1.z() <= _z_inf && p2.z() <= _z_inf)
    {
      return color;
    }
    else
    {
      // Sort input points.
      const ggo::point3d_float & p_inf = p1.z() <= p2.z() ? p1 : p2;
      const ggo::point3d_float & p_sup = p1.z() >= p2.z() ? p1 : p2;
      GGO_ASSERT(p_inf.z() <= p_sup.z());

      if (p_inf.z() >= _z_inf && p_sup.z() <= _z_sup)
      {
        // Both points are in the fog.
        GGO_ASSERT(p_inf.z() <= _z_sup);
        GGO_ASSERT(p_sup.z() >= _z_inf);
      
        return map_color(color, p1, p2);
      }
      else
      {
        ggo::vector3d_float dir(p_sup - p_inf);
        dir.normalize();
          
        if (p_sup.z() <= _z_sup)
        {
          // A point under the fog and the other one inside.
          GGO_ASSERT(p_inf.z() <= _z_inf && p_sup.z() >= _z_inf && p_sup.z() <= _z_sup);
              
          float t = (_z_inf - p_inf.z()) / dir.z();
          ggo::point3d_float p = p_inf + t * dir;
              
          GGO_ASSERT(std::abs(p.z() - _z_inf) <= 0.001f);
            
          return map_color(color, p, p_sup);
        }
        else
        if (p_inf.z() >= _z_inf)
        {
          // A point above the fog and the other one inside.
          GGO_ASSERT(p_sup.z() >= _z_sup && p_inf.z() >= _z_inf && p_inf.z() <= _z_sup);
              
          float t = (_z_sup - p_inf.z()) / dir.z();
          ggo::point3d_float p = p_inf + t * dir;
              
          GGO_ASSERT(std::abs(p.z() - _z_sup) <= 0.001f);
              
          return map_color(color, p, p_inf);
        }
        else
        {
          // A point above the fog and the other under it.
          GGO_ASSERT(p_inf.z() <= _z_inf && p_sup.z() >= _z_sup);
              
          float t1 = (_z_inf - p_inf.z()) / dir.z();
          float t2 = (_z_sup - p_inf.z()) / dir.z();
          ggo::point3d_float p1 = p_inf + t1 * dir;
          ggo::point3d_float p2 = p_inf + t2 * dir;
              
          GGO_ASSERT(std::abs(p1.z() -_z_inf) < 0.001f);
          GGO_ASSERT(std::abs(p2.z() -_z_sup) < 0.001f);
              
          return map_color(color, p1, p2);
        }
      }
    }
  }
      
  //////////////////////////////////////////////////////////////
  ggo::color z_fog::process_background_ray(const ggo::ray3d_float & ray, const ggo::color & color) const
  {
    if (std::abs(ray.dir().z()) < 0.001)
    {
      if (ray.pos().z() > _z_inf && ray.pos().z() < _z_sup)
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
      float t_inf = (_z_inf - ray.pos().z()) / ray.dir().z();
      float t_sup = (_z_sup - ray.pos().z()) / ray.dir().z();
        
      if (t_inf <= 0 && t_sup <= 0)
      {
        return color;
      }
      else
      if (t_inf <= 0 && t_sup >= 0)
      {
        ggo::point3d_float p = ray.pos() + t_sup * ray.dir();
            
        return map_color(color, p, ray.pos());
      }
      else
      if (t_inf >= 0 && t_sup <= 0)
      {
        ggo::point3d_float p = ray.pos() + t_inf * ray.dir();
            
        return map_color(color, p, ray.pos());
      }
      else
      {
        GGO_ASSERT(t_inf >= 0 && t_sup >= 0);
           
        ggo::point3d_float p1 = ray.pos() + t_inf * ray.dir();
        ggo::point3d_float p2 = ray.pos() + t_sup * ray.dir();
            
        return map_color(color, p1, p2);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::color z_fog::map_color(const ggo::color & color, const ggo::point3d_float & p1, const ggo::point3d_float & p2) const
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
