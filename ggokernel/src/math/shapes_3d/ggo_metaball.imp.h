namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  metaball<data_t>::metaball(float threshold)
  :
  _threshold(threshold)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void metaball<data_t>::add_influence_sphere(const ggo::set3<data_t> & pos)
  {
    _influence_spheres.push_back(pos);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<typename ggo::metaball<data_t>::intersection_info> metaball<data_t>::get_intersections(const ggo::ray3d<data_t> & ray) const
  {
    std::vector<intersection_info> intersections;

    // Build the intersections list.
    for (const auto & influence_sphere : _influence_spheres)
    {
      // Check for ray / influence sphere intersection.
      ggo::sphere3d<data_t> sphere(influence_sphere, 1);
      data_t dist_inf, dist_sup;
      if (sphere.intersect_ray(ray, dist_inf, dist_sup) == false)
      {
        continue;
      }

      if (dist_inf <= 0 || dist_sup <= 0 || dist_inf == dist_sup)
      {
        continue;
      }

      GGO_ASSERT(dist_inf <= dist_sup);

      intersection_info intersection_info_inf;
      intersection_info_inf._entry = true;
      intersection_info_inf._center = &influence_sphere;
      intersection_info_inf._dist = dist_inf;
      intersections.push_back(intersection_info_inf);

      intersection_info intersection_info_sup;
      intersection_info_sup._entry = false;
      intersection_info_sup._center = &influence_sphere;
      intersection_info_sup._dist = dist_sup;
      intersections.push_back(intersection_info_sup);
    }

    // Sort the intersection list.
    std::sort(intersections.begin(), intersections.end(), [](const intersection_info & i1, const intersection_info & i2) { return i1._dist < i2._dist; });

    return intersections;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool metaball<data_t>::update_active_list(const float dist,
                                            const std::vector<typename ggo::metaball<data_t>::intersection_info> & intersections,
                                            std::vector<const intersection_info*> & active_list,
                                            typename std::vector<intersection_info>::const_iterator & intersection_it)
  {
    while (dist >= intersection_it->_dist)
    {
      if (intersection_it->_entry == true)
      {
#ifdef GGO_METABALLS_DEBUG
        std::cout << "adding center " << intersection_it->_center << " into active list at t=" << dist << std::endl;
#endif
        // We are entering an influence sphere, add it in the active list.
        active_list.push_back(&(*intersection_it));
      }
      else
      {
#ifdef GGO_METABALLS_DEBUG
        std::cout << "removing center " << intersection_it->_center << " from active list at t=" << dist << std::endl;
#endif
        // We are leaving an influence sphere, remove it from the active list.
        ggo::remove_first_if(active_list, [&](const intersection_info * intersection) { return intersection->_center == intersection_it->_center; });
      }

      ++intersection_it;

      if (intersection_it == intersections.end())
      {
#ifdef GGO_METABALLS_DEBUG
        std::cout << "reached the end of intersection infos list at t=" << dist << std::endl;
#endif
        return false;
      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool metaball<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    // Build the intersections list.
    auto intersections = get_intersections(ray);
    if (intersections.empty() == true)
    {
      return false;
    }

#ifdef GGO_METABALLS_DEBUG
    std::cout << "****** intersection infos list size: " << intersection_list.size() << std::endl;
    for (const auto & intersection_info : intersection_list)
    {
      std::cout << "t: " << intersection_info._t << ", entry: " << intersection_info._entry << ", sphere: " << intersection_info._sphere << std::endl;
    }
#endif

    // Inch over the ray while maintaining an active intersection infos list.
    GGO_ASSERT(intersections.front()._entry == true);

    std::vector<intersection_info>::const_iterator intersection_it = intersections.begin();
    dist = 0;

    std::vector<const intersection_info*> active_list;

    while (true)
    {
      // Update the active intersection infos list.
      if (update_active_list(dist, intersections, active_list, intersection_it) == false)
      {
        return false;
      }

      // There is no active intersection info, we can jump to the next one.
      if (active_list.empty() == true)
      {
        GGO_ASSERT(intersection_it->_entry == true);

        dist = intersection_it->_dist;
#ifdef GGO_METABALLS_DEBUG
        std::cout << "jumping to dist=" << dist << ", adding sphere " << intersection_it->_center << " into active list" << std::endl;
#endif	
        active_list.push_back(&(*intersection_it));

        ++intersection_it;

        GGO_ASSERT(intersection_it != intersections.end());
      }

      // Compute field potential.
      data_t v = 0;
      ggo::set3<data_t> pos(ray.pos() + dist * ray.dir());
      for (const auto * intersection : active_list)
      {
        const ggo::set3<data_t> * center = intersection->_center;

        GGO_ASSERT(intersection->_entry == true);

        ggo::set3<data_t> diff = pos - *center;

        data_t hypot = diff.get_length();
        data_t influence = hypot * hypot - 2 * hypot + 1; // r^4 - 2r^2 + 1
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
        ggo::set3<data_t> dir(0, 0, 0);
        for (const auto * intersection : active_list)
        {
          const ggo::set3<data_t> * center = intersection->_center;

          GGO_ASSERT(intersection->_entry == true);

          ggo::set3<data_t> diff = pos - *center;

          data_t hypot = diff.get_length();
          data_t influence = hypot * hypot - 2 * hypot + 1; // r^4 - 2r^2 + 1
          data_t r = std::sqrt(hypot);
          GGO_ASSERT(hypot <= 1.001);

          diff /= r; // Normalize.
          GGO_ASSERT(diff.is_normalized(0.01f) == true);
          diff *= influence;
          dir += diff;
        }

        // Make sure the normal is well oriented.
        data_t overflow = 0.001f + ggo::dot(dir, ray.dir());
        if (overflow > 0)
        {
          dir -= overflow * ray.dir();
        }

        normal = ggo::ray3d<data_t>(pos, dir);

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
