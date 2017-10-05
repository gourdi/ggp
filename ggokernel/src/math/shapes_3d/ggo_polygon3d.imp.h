namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  polygon3d<T>::polygon3d(const std::vector<vertex<T>> & vertices, const std::vector<face> & faces)
  :
  _vertices(vertices),
  _faces(faces)
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool polygon3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    const face * hit_face = nullptr;

    dist = std::numeric_limits<T>::max();

    // Parse all the faces.
    for (const auto & face : _faces)
    {
      const ggo::pos3<T> &      v1 = _vertices[face._v1]._pos;
      const ggo::pos3<T> &      v2 = _vertices[face._v2]._pos;
      const ggo::pos3<T> &      v3 = _vertices[face._v3]._pos;
      ggo::triangle3d<T, false> triangle(v1, v2, v3);

      T dist_cur = 0;
      ggo::ray3d<T> normal_cur;
          
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
    const ggo::pos3<T> & v1 = _vertices[hit_face->_v1]._pos;
    const ggo::pos3<T> & v2 = _vertices[hit_face->_v2]._pos;
    const ggo::pos3<T> & v3 = _vertices[hit_face->_v3]._pos;
    
    const T m[3][3] = {{v1.x(), v2.x(), v3.x()}, 
                       {v1.y(), v2.y(), v3.y()}, 
                       {v1.z(), v2.z(), v3.z()}};
    const T c[3] = {normal.pos().x(), normal.pos().y(), normal.pos().z()};
    T s[3] = {0, 0, 0};
    
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
  template <typename T>
  polygon3d<T> polygon3d<T>::create_sphere(T radius, int horz_steps, int vert_steps)
  {
    // The vertices.
    std::vector<ggo::vertex<T>> vertices;
    
    vertices.emplace_back(ggo::pos3<T>(T(0), T(0), radius), ggo::vec3<T>(T(0), T(0), T(1))); // North pole.

    for (int phi_step = 1; phi_step < vert_steps - 1; ++phi_step)
    {
      T phi = pi<T>() * phi_step / (vert_steps - 1);
      T cos_phi = std::cos(phi);
      T sin_phi = std::sin(phi);
      
      for (int theta_step = 0; theta_step < horz_steps; ++theta_step)
      {
        T theta = 2 * pi<T>() * theta_step / horz_steps;
        T x = sin_phi * std::cos(theta);
        T y = sin_phi * std::sin(theta);
        T z = cos_phi;
        
        ggo::vec3<T> p(x, y, z);
        vertices.emplace_back(radius * p, p);
      }
    }
    
    vertices.emplace_back(ggo::pos3<T>(T(0), T(0), -radius), ggo::vec3<T>(T(0), T(0), T(-1))); // South pole.

    // The faces.
    std::vector<ggo::polygon3d<T>::face> faces;
    
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

    return polygon3d<T>(vertices, faces);
  }
}
