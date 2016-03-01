#include "ggo_metaballs_artist.h"
#include <iostream>
#include <algorithm>
#include <ggo_object3d.h>
#include <ggo_background3d_color.h>

//#define GGO_METABALLS_DEBUG

namespace 
{
  class ggo_metaball : public ggo::raytracable_shape3d_abc<float>
	{
	public:
    
                ggo_metaball(const std::vector<ggo::point3d_float> & centers, float threshold);
      
		bool	      intersect_ray(const ggo::ray3d<float> & ray, float & dist, ggo::ray3d<float> & normal) const override;
    bool	      is_convex() const override { return false; }
    std::string desc() const override { return "metaball"; }

	public:

		const std::vector<ggo::point3d_float> &	_centers;
		float							 		                  _threshold;
	};

	struct ggo_intersection_info
	{
		bool 						            _entry;
		const ggo::point3d_float *  _center;
		float 						          _t;
	};

	void 	remove_intersection_info(std::vector<const ggo_intersection_info*> & list, const ggo::point3d_float * center)
  {
    GGO_ASSERT(list.empty() == false);
	
    // Remove entry intersection info.
    for (std::vector<const ggo_intersection_info*>::iterator it = list.begin(); it != list.end(); ++it)
    {
      if ((*it)->_center == center)
      {
        GGO_ASSERT((*it)->_entry == true);
        list.erase(it);
        return;
      }
    }
	
    GGO_FAIL();
  }
}

namespace
{ 
  //////////////////////////////////////////////////////////////
  ggo_metaball::ggo_metaball(const std::vector<ggo::point3d_float> & centers, float threshold)
  :
  _centers(centers),
  _threshold(threshold)
  {
  }

  //////////////////////////////////////////////////////////////
  bool ggo_metaball::intersect_ray(const ggo::ray3d<float> & ray, float & dist, ggo::ray3d<float> & normal) const
  {
    std::vector<ggo_intersection_info> intersection_list;
    
    // Build the intersections list.
    for (const auto & center : _centers)
    {
      // Check for ray / influence sphere intersection.
      const ggo::vector3d_float & dir = ray.dir();
      const ggo::point3d_float & pos = ray.pos();

      float dx = pos.x() - center.x();
      float dy = pos.y() - center.y();
      float dz = pos.z() - center.z();

      float deg2 = dir.x()*dir.x() + dir.y()*dir.y() + dir.z()*dir.z();
      float deg1 = 2*(dir.x()*dx + dir.y()*dy + dir.z()*dz);
      float deg0 = dx*dx + dy*dy + dz*dz - 1;

      float t1, t2;
      if (ggo::quadratic_solve(deg2, deg1, deg0, t1, t2) == false)
      {
        continue;
      }

      if (t1 <= 0 || t2 <= 0 || t1 == t2)
      {
        continue;
      }
      
      // Store intersection info in a list.
      if (t1 > t2)
      {
        std::swap(t1, t2);
      }
      GGO_ASSERT(t1 <= t2);
      
      ggo_intersection_info intersection_info_inf;
      intersection_info_inf._entry = true;
      intersection_info_inf._center = &center;
      intersection_info_inf._t = t1;
      intersection_list.push_back(intersection_info_inf);
      
      ggo_intersection_info intersection_info_sup;
      intersection_info_sup._entry = false;
      intersection_info_sup._center = &center;
      intersection_info_sup._t = t2;
      intersection_list.push_back(intersection_info_sup);
    }
    
    if (intersection_list.empty() == true)
    {
      return false;
    }

    // Sort the intersection list.
    std::sort(intersection_list.begin(), intersection_list.end(), [](const ggo_intersection_info & i1, const ggo_intersection_info & i2) { return i1._t < i2._t; });
    
  #ifdef GGO_METABALLS_DEBUG
    std::cout << "****** intersection infos list size: " << intersection_list.size() << std::endl;
    for (const auto & intersection_info : intersection_list)
    {
      std::cout << "t: " << intersection_info._t << ", entry: " << intersection_info._entry << ", sphere: " << intersection_info._sphere << std::endl;
    }
  #endif
    
    // Inch over the ray while maintaining an active intersection infos list.
    GGO_ASSERT(intersection_list.front()._entry == true);
    
    std::vector<ggo_intersection_info>::const_iterator intersection_it = intersection_list.begin();
    dist = 0;
    
    std::vector<const ggo_intersection_info*> active_list;
    
    while (true)
    {
      // Update the active intersection infos list.
      while (dist >= intersection_it->_t)
      {
        if (intersection_it->_entry == true)
        {
  #ifdef GGO_METABALLS_DEBUG
          std::cout << "adding center " << intersection_it->_center << " into active list at t=" << dist << std::endl;
  #endif
          active_list.push_back(&(*intersection_it));
        }
        else
        {
  #ifdef GGO_METABALLS_DEBUG
          std::cout << "removing center " << intersection_it->_center << " from active list at t=" << dist << std::endl;
  #endif
          remove_intersection_info(active_list, intersection_it->_center);
        }
        
        ++intersection_it;
        
        if (intersection_it == intersection_list.end())
        {
  #ifdef GGO_METABALLS_DEBUG
          std::cout << "reached the end of intersection infos list at t=" << dist << std::endl;
  #endif
          return false;
        }
      }
      
      // There is no active intersection info, we can jump to the next one.
      if (active_list.empty() == true)
      {
        GGO_ASSERT(intersection_it->_entry == true);

        dist = intersection_it->_t;
  #ifdef GGO_METABALLS_DEBUG
        std::cout << "jumping to dist=" << dist << ", adding sphere " << intersection_it->_center << " into active list" << std::endl;
  #endif	
        active_list.push_back(&(*intersection_it));

        ++intersection_it;
        
        GGO_ASSERT(intersection_it != intersection_list.end());
      }

      // Compute field potential.
      float v = 0;
      ggo::point3d_float pos(ray.pos() + dist * ray.dir());
      for (std::vector<const ggo_intersection_info*>::const_iterator it = active_list.begin(); it != active_list.end(); ++it)
      {
        const ggo_intersection_info * intersection_info = *it;
        const ggo::point3d_float * center = intersection_info->_center;
        
        GGO_ASSERT(intersection_info->_entry == true);
        
        ggo::vector3d_float diff = pos - *center;
        
        float hypot = diff.get_length();
        float influence = hypot * hypot - 2 * hypot + 1; // r^4 - 2r^2 + 1
        GGO_ASSERT(hypot <= 1.001);
        
        v += influence;
      }

      // Check for intersection of the surface
      if (v > _threshold)
      {
  #ifdef GGO_METABALLS_DEBUG
        std::cout << "intersection detected at t=" << t << "(" << active_list.size() << " active sphere(s))" << std::endl;
  #endif
        
        // Compute normal direction.
        ggo::vector3d_float dir(0, 0, 0);
        for (std::vector<const ggo_intersection_info*>::const_iterator it = active_list.begin(); it != active_list.end(); ++it)
        {
          const ggo_intersection_info * intersection_info = *it;
          const ggo::point3d_float * center = intersection_info->_center;

          GGO_ASSERT(intersection_info->_entry == true);

          ggo::vector3d_float diff = pos - *center;

          float hypot = diff.get_length();
          float influence = hypot * hypot - 2 * hypot + 1; // r^4 - 2r^2 + 1
          float r = std::sqrt(hypot);
          GGO_ASSERT(hypot <= 1.001);

          diff /= r; // Normalize.
          GGO_ASSERT(diff.is_normalized(0.01f) == true);
          diff *= influence;
          dir += diff;
        }
        
        // Make sure the normal is well oriented.
        float overflow = 0.001f + ggo::dot(dir, ray.dir());
        if (overflow > 0)
        {
          dir -= overflow * ray.dir(); 
        }
              
        normal = ggo::ray3d_float(pos, dir);
        
        return true;
      }
      
      if (v < 0.5)
      {
        dist += 0.1f;
      }
      else
      if (v < 0.7)
      {
        dist += 0.01f;
      }
      else
      {
        dist += 0.001f;
      }
    }
    
    return false;
  }
}

//////////////////////////////////////////////////////////////
ggo_metaballs_artist::ggo_metaballs_params::ggo_metaballs_params()
{
	float angle = ggo::rand_float(0, 2 * ggo::PI<float>());
	
	_threshold = ggo::rand_float(0.75f, 0.85f);
	_phong_factor = ggo::rand_float(2, 4);
	_phong_shininess = ggo::rand_float(10, 100);
	_light1 = ggo::point3d_float(0, 0, 1000);
	_light2 = ggo::point3d_float(1000 * std::cos(angle), 1000 * std::sin(angle), 1000);
	_background_color = ggo::color(ggo::rand_float(0.2f, 0.8f));
}

//////////////////////////////////////////////////////////////
ggo_metaballs_artist::ggo_metaballs_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
}	

//////////////////////////////////////////////////////////////
void ggo_metaballs_artist::render_bitmap(uint8_t * buffer, ggo::renderer_abc & renderer, const ggo_metaballs_params & params)
{
#ifdef GGO_METABALLS_DEBUG
	std::cout << "Threshold: " << metaball._threshold << std::endl;
#endif

  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(params._background_color));

  auto shape = std::make_shared<ggo_metaball>(params._centers, params._threshold);
  auto object = scene_builder.add_object(shape, params._background_color, true);
  object->set_phong_factor(params._phong_factor);
  object->set_phong_shininess(params._phong_shininess);

	scene_builder.add_sphere_light(ggo::color(0.8f), 0.1f, params._light1);
  scene_builder.add_sphere_light(ggo::color(0.8f), 0.1f, params._light2);

  renderer.render(buffer, get_render_width(), get_render_height(), scene_builder);
}

