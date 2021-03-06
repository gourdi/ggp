namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  polygon3d<data_t>::polygon3d(const std::vector<vertex<data_t>> & vertices, const std::vector<face> & faces)
  :
  _vertices(vertices),
  _faces(faces)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool polygon3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    const face * hit_face = nullptr;

    dist = std::numeric_limits<data_t>::max();

    // Parse all the faces.
    for (const auto & face : _faces)
    {
      const ggo::pos3<data_t> &      v1 = _vertices[face._v1]._pos;
      const ggo::pos3<data_t> &      v2 = _vertices[face._v2]._pos;
      const ggo::pos3<data_t> &      v3 = _vertices[face._v3]._pos;
      ggo::triangle3d<data_t, false> triangle(v1, v2, v3);

      data_t dist_cur = 0;
      ggo::ray3d<data_t> normal_cur;
          
      if ((triangle.intersect_ray(ray, dist_cur, normal_cur) == true) && (dist_cur < dist))
      {
        dist          = dist_cur;
        normal.pos()  = normal_cur.pos();
        hit_face      = &face;
      }
    }

    if (hit_face == nullptr)
    {
      return false;
    }
    
    // Interpolate the normal smoothly.
    const ggo::pos3<data_t> & v1 = _vertices[hit_face->_v1]._pos;
    const ggo::pos3<data_t> & v2 = _vertices[hit_face->_v2]._pos;
    const ggo::pos3<data_t> & v3 = _vertices[hit_face->_v3]._pos;
    
    const data_t m[3][3] = { 
      { v1.x(), v2.x(), v3.x() },
      { v1.y(), v2.y(), v3.y() },
      { v1.z(), v2.z(), v3.z() } };
    const data_t c[3] = {normal.pos().x(), normal.pos().y(), normal.pos().z()};
    data_t s[3] = { 0, 0, 0 };
    
    if (linsolve3d(m, c, s) == false)
    {
      return false;
    }

    GGO_ASSERT(s[0] >= -0.0001 && s[0] <= 1.0001);
    GGO_ASSERT(s[1] >= -0.0001 && s[1] <= 1.0001);
    GGO_ASSERT(s[2] >= -0.0001 && s[2] <= 1.0001);
    GGO_ASSERT(std::abs(s[0] + s[1] + s[2] - 1) < 0.0001);
    
    normal.set_dir(s[0] * _vertices[hit_face->_v1]._normal +
                   s[1] * _vertices[hit_face->_v2]._normal +
                   s[2] * _vertices[hit_face->_v3]._normal);

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<box3d_data<data_t>> polygon3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    if (_vertices.empty() == true)
    {
      return {};
    }

    auto it = _vertices.begin();

    ggo::pos3<data_t> world_vertex = basis.point_from_local_to_world(it->_pos);
    data_t x_min = world_vertex.x();
    data_t x_max = world_vertex.x();
    data_t y_min = world_vertex.y();
    data_t y_max = world_vertex.y();
    data_t z_min = world_vertex.z();
    data_t z_max = world_vertex.z();

    ++it;

    for (; it != _vertices.end(); ++it)
    {
      world_vertex = basis.point_from_local_to_world(it->_pos);

      x_min = std::min(x_min, world_vertex.x());
      x_max = std::max(x_max, world_vertex.x());
      y_min = std::min(y_min, world_vertex.y());
      y_max = std::max(y_max, world_vertex.y());
      z_min = std::min(z_min, world_vertex.z());
      z_max = std::max(z_max, world_vertex.z());
    }

    return box3d_data<data_t>(x_min, x_max, y_min, y_max, z_min, z_max);
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  polygon3d<data_t> polygon3d<data_t>::create_sphere(data_t radius, int horz_steps, int vert_steps)
  {
    // The vertices.
    std::vector<ggo::vertex<data_t>> vertices;
    
    vertices.emplace_back(ggo::pos3<data_t>(data_t(0), data_t(0), radius), ggo::vec3<data_t>(data_t(0), data_t(0), data_t(1))); // North pole.

    for (int phi_step = 1; phi_step < vert_steps - 1; ++phi_step)
    {
      data_t phi = pi<data_t>() * phi_step / (vert_steps - 1);
      data_t cos_phi = std::cos(phi);
      data_t sin_phi = std::sin(phi);
      
      for (int theta_step = 0; theta_step < horz_steps; ++theta_step)
      {
        data_t theta = 2 * pi<data_t>() * theta_step / horz_steps;
        data_t x = sin_phi * std::cos(theta);
        data_t y = sin_phi * std::sin(theta);
        data_t z = cos_phi;
        
        ggo::vec3<data_t> p(x, y, z);
        vertices.emplace_back(radius * p, p);
      }
    }
    
    vertices.emplace_back(ggo::pos3<data_t>(data_t(0), data_t(0), -radius), ggo::vec3<data_t>(data_t(0), data_t(0), data_t(-1))); // South pole.

    // The faces.
    std::vector<ggo::polygon3d<data_t>::face> faces;
    
    for (int i = 0; i < horz_steps; ++i) // Faces connected to the North pole.
    {
      faces.emplace_back(0, 1 + i, 1 + ((i + 1) % horz_steps));
    }
    
    for (int j = 1; j < vert_steps - 2; ++j) // Inner faces.
    {
      for (int i = 0; i < horz_steps; ++i)
      {
        int v1 = 1 + (j - 1) * horz_steps + i;
        int v2 = 1 + (j - 1) * horz_steps + ((i + 1) % horz_steps);
        int v3 = 1 + j * horz_steps + i;
        int v4 = 1 + j * horz_steps + ((i + 1) % horz_steps);

        faces.emplace_back(v3, v2, v1);
        faces.emplace_back(v2, v3, v4);
      }
    }

    int offset = 1 + (vert_steps - 3) * horz_steps;
    for (int i = 0; i < horz_steps; ++i) // Faces connected to the South pole.
    {
      faces.emplace_back(static_cast<int>(vertices.size()) - 1, offset + i, offset + ((i + 1) % horz_steps));
    }

    return polygon3d<data_t>(vertices, faces);
  }
}
