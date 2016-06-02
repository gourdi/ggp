namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t metaball<data_t>::influence_sphere::evaluate(const ggo::set3<data_t> & pos) const
  {
    // f = potential * ((hypot / r^2)^2 - 2 * hypot / r^2 + 1)
    // f = potential * (hypot_norm^2 - 2 * hypot_norm + 1) if hypot_norm = hypot / r^2

    data_t hypot_norm = ggo::hypot(pos, _sphere.center()) * _inv_squared_radius;
    GGO_ASSERT(hypot_norm <= data_t(1.001));

    data_t influence = ggo::square(hypot_norm - 1); // x^2 - 2x + 1 = (x - 1)^2
    influence *= _potential;
    
    GGO_ASSERT(influence >= data_t(0.));

    return influence;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  metaball<data_t>::metaball(float threshold)
  :
  _threshold(threshold)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void metaball<data_t>::add_influence_sphere(const ggo::sphere3d<data_t> & sphere, data_t potential)
  {
    _influence_spheres.push_back({ sphere, potential, 1 / ggo::square(sphere.radius()) });
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
      data_t dist_inf, dist_sup;
      if (influence_sphere._sphere.intersect_ray(ray, dist_inf, dist_sup) == false)
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
      intersection_info_inf._influence_sphere = &influence_sphere;
      intersection_info_inf._dist = dist_inf;
      intersections.push_back(intersection_info_inf);

      intersection_info intersection_info_sup;
      intersection_info_sup._entry = false;
      intersection_info_sup._influence_sphere = &influence_sphere;
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
        ggo::remove_first_if(active_list, [&](const intersection_info * intersection) { return intersection->_influence_sphere == intersection_it->_influence_sphere; });
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
  data_t metaball<data_t>::compute_field_potential(const ggo::set3<data_t> & pos, 
                                                   const std::vector<const intersection_info*> & active_list)
  {
    data_t v = 0;

    for (const auto * intersection : active_list)
    {
      GGO_ASSERT(intersection->_entry == true);

      v += intersection->_influence_sphere->evaluate(pos);
    }

    return v;
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

      // Compute field potential
      normal.pos() = ray.pos() + dist * ray.dir();
      dist += data_t(0.001);

      auto compute_field_potential = [&](float acc, const intersection_info * intersection)
      {
        return acc + intersection->_influence_sphere->evaluate(normal.pos());
      };
      data_t v = ggo::accumulate(active_list, compute_field_potential, data_t(0));

      // Check if inside the surface. If so => intersection.
      if (v > _threshold)
      {
#ifdef GGO_METABALLS_DEBUG
        std::cout << "intersection detected at t=" << t << "(" << active_list.size() << " active sphere(s))" << std::endl;
#endif

        // Use previous distance, to avoid self-intersection.
        dist -= data_t(0.001);
        normal.pos() = ray.pos() + dist * ray.dir();

        // Compute normal direction.
        ggo::set3<data_t> dir(0, 0, 0);
        for (const auto * intersection : active_list)
        {
          GGO_ASSERT(intersection->_entry == true);

          const influence_sphere * influence_sphere = intersection->_influence_sphere;

          ggo::set3<data_t> diff = normal.pos() - influence_sphere->_sphere.center();

          // df/dx = 4 * potential * x * (hypot / r^2 - 1) / r^2
          // df/dy = 4 * potential * y * (hypot / r^2 - 1) / r^2
          // df/dz = 4 * potential * z * (hypot / r^2 - 1) / r^2
          ggo::set3<data_t> cur_grad = (1 - diff.get_hypot() * influence_sphere->_inv_squared_radius) * diff;
          cur_grad *= influence_sphere->_potential * influence_sphere->_inv_squared_radius; // Don't multiply by 4 since normal is normalized. 

          dir += cur_grad;
        }

        normal.set_dir(dir);

#if 0 // The following code does not rely on analytic derivaties but on neighborhood sampling.
        const float eps = 0.001f;
        float x_inf = compute_field_potential({ pos.x() - eps, pos.y(), pos.z() }, active_list);
        float x_sup = compute_field_potential({ pos.x() + eps, pos.y(), pos.z() }, active_list);
        float y_inf = compute_field_potential({ pos.x(), pos.y() - eps, pos.z() }, active_list);
        float y_sup = compute_field_potential({ pos.x(), pos.y() + eps, pos.z() }, active_list);
        float z_inf = compute_field_potential({ pos.x(), pos.y(), pos.z() - eps }, active_list);
        float z_sup = compute_field_potential({ pos.x(), pos.y(), pos.z() + eps }, active_list);
        
        normal.set_dir(ggo::vector3d_float(x_inf - x_sup, y_inf - y_sup, z_inf - z_sup));
#endif

        return true;
      }
    }

    return false;
  }
}
