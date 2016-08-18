#include "ggo_delaunay_triangulation.h"
#include <ggo_link.h>
#include <ggo_kernel.h>
#include <ggo_shapes2d.h>

#define EPSILON T(0.0001)

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool delaunay_triangle<T>::is_valid() const
  {
    if (_v1 == _v2) { return false; }
    if (_v1 == _v3) { return false; }
    if (_v2 == _v3) { return false; }
    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool delaunay_triangle<T>::compare(const delaunay_triangle & other) const
  {
    if ((_v1 != other._v1) && (_v1 != other._v2) && (_v1 != other._v3)) 
    {
      return false;
    }
    if ((_v2 != other._v1) && (_v2 != other._v2) && (_v2 != other._v3)) 
    {
      return false;
    }
    if ((_v3 != other._v1) && (_v3 != other._v2) && (_v3 != other._v3)) 
    {
      return false;
    }
    return true;
  }

  template<class T>
  using edge = ggo::link<const ggo::pos2<T> *>;
} 

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool find_common_edge(const ggo::delaunay_triangle<T> & triangle1, const ggo::delaunay_triangle<T> & triangle2, ggo::edge<T> & common_edge)
  {
    ggo::edge<T> edge1_1(triangle1._v1, triangle1._v2);
    ggo::edge<T> edge1_2(triangle1._v2, triangle1._v3);
    ggo::edge<T> edge1_3(triangle1._v3, triangle1._v1);
    ggo::edge<T> edge2_1(triangle2._v1, triangle2._v2);
    ggo::edge<T> edge2_2(triangle2._v2, triangle2._v3);
    ggo::edge<T> edge2_3(triangle2._v3, triangle2._v1);

    if ((edge1_1 == edge2_1) || (edge1_1 == edge2_2) || (edge1_1 == edge2_3))
    {
      common_edge = edge1_1;
      return true;
    }

    if ((edge1_2 == edge2_1) || (edge1_2 == edge2_2) || (edge1_2 == edge2_3))
    {
      common_edge = edge1_2;
      return true;
    }

    if ((edge1_3 == edge2_1) || (edge1_3 == edge2_2) || (edge1_3 == edge2_3))
    {
      common_edge = edge1_3;
      return true;
    }
    
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T compute_angle(const ggo::pos2<T> * common, const ggo::pos2<T> * pt1, const ggo::pos2<T> * pt2)
  {
    T dx1 = pt1->template get<0>() - common->template get<0>();
    T dy1 = pt1->template get<1>() - common->template get<1>();
    T dx2 = pt2->template get<0>() - common->template get<0>();
    T dy2 = pt2->template get<1>() - common->template get<1>();

    T len1 = std::sqrt(dx1 * dx1 + dy1 * dy1);
    T len2 = std::sqrt(dx2 * dx2 + dy2 * dy2);

    // Normalize.
    dx1 /= len1;
    dy1 /= len1;
    dx2 /= len2;
    dy2 /= len2;

    // We need to clamp, otherwise dot is 1 or -1 which may result to a NaN when computin acos.
    T dot = ggo::clamp(dx1 * dx2 + dy1 * dy2, T(-0.999999), T(0.999999));

    return std::acos(dot);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T compute_opposite_angle(const ggo::delaunay_triangle<T> & triangle, const ggo::edge<T> & opposite_edge)
  {
    GGO_ASSERT(opposite_edge._v1 != opposite_edge._v2);
    GGO_ASSERT(triangle.is_valid());
    GGO_ASSERT((triangle._v1 == opposite_edge._v1) || (triangle._v2 == opposite_edge._v1) || (triangle._v3 == opposite_edge._v1));
    GGO_ASSERT((triangle._v1 == opposite_edge._v2) || (triangle._v2 == opposite_edge._v2) || (triangle._v3 == opposite_edge._v2));
    
    if (ggo::edge<T>(triangle._v1, triangle._v2) == opposite_edge)
    {
      return compute_angle(triangle._v3, triangle._v1, triangle._v2);
    }
    else
    if (ggo::edge<T>(triangle._v2, triangle._v3) == opposite_edge)
    {
      return compute_angle(triangle._v1, triangle._v2, triangle._v3);
    }
    else
    if (ggo::edge<T>(triangle._v3, triangle._v1) == opposite_edge)
    {
      return compute_angle(triangle._v2, triangle._v3, triangle._v1);
    }
    else
    {
      GGO_FAIL();
      return -1;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool are_triangles_valid(const ggo::delaunay_triangle<T> & triangle1, const ggo::delaunay_triangle<T> & triangle2, const ggo::edge<T> & common_edge, T epsilon)
  {
    GGO_ASSERT(triangle1.is_valid());
    GGO_ASSERT(triangle2.is_valid());

    T angle1 = compute_opposite_angle(triangle1, common_edge);
    T angle2 = compute_opposite_angle(triangle2, common_edge);
    
    GGO_ASSERT(angle1 >= 0);
    GGO_ASSERT(angle2 >= 0);

    return angle1 + angle2 < ggo::pi<T>() + epsilon;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void flip_triangle(ggo::delaunay_triangle<T> & triangle1, ggo::delaunay_triangle<T> & triangle2, const ggo::edge<T> & common_edge)
  {
    GGO_ASSERT(triangle1.compare(triangle2) == false);
    GGO_ASSERT(triangle1.is_valid() == true);
    GGO_ASSERT(triangle2.is_valid() == true);
    GGO_ASSERT(common_edge._v1 != common_edge._v2);
    GGO_ASSERT((triangle1._v1 == common_edge._v1) || (triangle1._v2 == common_edge._v1) || (triangle1._v3 == common_edge._v1));
    GGO_ASSERT((triangle1._v1 == common_edge._v2) || (triangle1._v2 == common_edge._v2) || (triangle1._v3 == common_edge._v2));
    GGO_ASSERT((triangle2._v1 == common_edge._v1) || (triangle2._v2 == common_edge._v1) || (triangle2._v3 == common_edge._v1));
    GGO_ASSERT((triangle2._v1 == common_edge._v2) || (triangle2._v2 == common_edge._v2) || (triangle2._v3 == common_edge._v2));

    const ggo::pos2<T> * opposite1 = NULL;
    const ggo::pos2<T> * opposite2 = NULL;

    // Build new triangles.
    if ((triangle1._v1 != common_edge._v1) && (triangle1._v1 != common_edge._v2)) { opposite1 = triangle1._v1; }
    if ((triangle1._v2 != common_edge._v1) && (triangle1._v2 != common_edge._v2)) { opposite1 = triangle1._v2; }
    if ((triangle1._v3 != common_edge._v1) && (triangle1._v3 != common_edge._v2)) { opposite1 = triangle1._v3; }

    if ((triangle2._v1 != common_edge._v1) && (triangle2._v1 != common_edge._v2)) { opposite2 = triangle2._v1; }
    if ((triangle2._v2 != common_edge._v1) && (triangle2._v2 != common_edge._v2)) { opposite2 = triangle2._v2; }
    if ((triangle2._v3 != common_edge._v1) && (triangle2._v3 != common_edge._v2)) { opposite2 = triangle2._v3; }

    GGO_ASSERT(opposite1 != NULL);
    GGO_ASSERT(opposite2 != NULL);
    GGO_ASSERT(opposite1 != opposite2);

    triangle1._v1 = common_edge._v1;
    triangle1._v2 = opposite1;
    triangle1._v3 = opposite2;

    triangle2._v1 = common_edge._v2;
    triangle2._v2 = opposite1;
    triangle2._v3 = opposite2;

    GGO_ASSERT(triangle1.compare(triangle2) == false);
    GGO_ASSERT(triangle1.is_valid() == true);
    GGO_ASSERT(triangle2.is_valid() == true);
    GGO_ASSERT(are_triangles_valid(triangle1, triangle2, ggo::edge<T>(opposite1, opposite2), T(0.01)) == true);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool find_triangle(std::list<ggo::delaunay_triangle<T>> & triangles, const ggo::delaunay_triangle<T> & triangle)
  {
    typename std::list<ggo::delaunay_triangle<T>>::iterator it;

    for (it = triangles.begin(); it != triangles.end(); ++it)
    {
      if (it->compare(triangle) == true)
      {
        return true;
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool remove_triangle(std::list<ggo::delaunay_triangle<T>> & triangles, const ggo::delaunay_triangle<T> & triangle)
  {
    typename std::list<ggo::delaunay_triangle<T>>::iterator it;

    for (it = triangles.begin(); it != triangles.end(); ++it)
    {
      if (it->compare(triangle) == true)
      {
        triangles.erase(it);
        return true;
      }
    }
    
    GGO_FAIL();

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool update_triangulation(std::list<ggo::delaunay_triangle<T>> & triangles)
  {
    ggo::edge<T> common_edge(nullptr, nullptr);
    
    for (auto it1 = triangles.begin(); it1 != triangles.end(); ++it1)
    {
      for (auto it2 = triangles.begin(); it2 != triangles.end(); ++it2)
      {	
        if ((it1 != it2) &&
            (find_common_edge(*it1, *it2, common_edge) == true) &&
            (are_triangles_valid(*it1, *it2, common_edge, T(0.001)) == false))
        {
          flip_triangle(*it1, *it2, common_edge);
          return true;
        }
      }
    }

    return false;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  void check_integrity(std::list<ggo::delaunay_triangle<T>> & triangles)
  {
#ifdef GGO_DEBUG
    for (auto it1 = triangles.begin(); it1 != triangles.end(); ++it1)
    {
      for (auto it2 = triangles.begin(); it2 != triangles.end(); ++it2)
      {	
        if ((it1 != it2) && (it1->compare(*it2) == true))
        {
          GGO_FAIL();
        }
      }
    }
#endif
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void add_new_vertex(const ggo::pos2<T> * new_vertex, std::list<ggo::delaunay_triangle<T>> & triangles, std::vector<const ggo::pos2<T> *> & vertices)
  {
    // First pass to check if the new vertex is not too close of another vertex.
    typename std::vector<const ggo::pos2<T> *>::iterator it_vertex;
    for (it_vertex = vertices.begin(); it_vertex != vertices.end(); ++it_vertex)
    {
      const ggo::pos2<T> * vertex = *it_vertex;
    
      // If the new point is too close from a point, discard it. 
      if (ggo::distance(*new_vertex, *vertex) < EPSILON)
      {
        return;
      }
    }
    
    // Second pass to check if the new vertex lies on an edge.
    auto it_triangle = triangles.begin();
    
    std::list<ggo::delaunay_triangle<T>> new_triangles;
    std::list<ggo::delaunay_triangle<T>> old_triangles;
    
    ggo::edge<T> edge(nullptr, nullptr);
    
    for (it_triangle = triangles.begin(); it_triangle != triangles.end(); ++it_triangle)
    {
      ggo::segment<T> s12(*(it_triangle->_v1), *(it_triangle->_v2));
      ggo::segment<T> s23(*(it_triangle->_v2), *(it_triangle->_v3));
      ggo::segment<T> s13(*(it_triangle->_v1), *(it_triangle->_v3));
      T d12 = s12.dist_to_point(new_vertex->template get<0>(), new_vertex->template get<1>());
      T d23 = s23.dist_to_point(new_vertex->template get<0>(), new_vertex->template get<1>());
      T d13 = s13.dist_to_point(new_vertex->template get<0>(), new_vertex->template get<1>());

      ggo::edge<T> current_edge(nullptr, nullptr);
      ggo::edge<T> edge1(nullptr, nullptr);
      ggo::edge<T> edge2(nullptr, nullptr);

      if (d12 < EPSILON)
      {
        current_edge = ggo::edge<T>(it_triangle->_v1, it_triangle->_v2);
        edge1 = ggo::edge<T>(it_triangle->_v1, it_triangle->_v3);
        edge2 = ggo::edge<T>(it_triangle->_v2, it_triangle->_v3);
      }
      else if (d23 < EPSILON)
      {
        current_edge = ggo::edge<T>(it_triangle->_v2, it_triangle->_v3);
        edge1 = ggo::edge<T>(it_triangle->_v1, it_triangle->_v2);
        edge2 = ggo::edge<T>(it_triangle->_v1, it_triangle->_v3);
      }
      else if (d13 < EPSILON)
      {
        current_edge = ggo::edge<T>(it_triangle->_v1, it_triangle->_v3);
        edge1 = ggo::edge<T>(it_triangle->_v1, it_triangle->_v2);
        edge2 = ggo::edge<T>(it_triangle->_v2, it_triangle->_v3);
      }
      
      if (current_edge._v1 != nullptr && current_edge._v2 != nullptr)
      {
        GGO_ASSERT(edge1._v1 != nullptr);
        GGO_ASSERT(edge1._v2 != nullptr);
        GGO_ASSERT(edge2._v1 != nullptr);
        GGO_ASSERT(edge2._v2 != nullptr);

        if (edge._v1 != nullptr && edge._v2 != nullptr && current_edge != edge)
        {
          GGO_TRACE("vertex detected to be on several edges, discarding it\n");
          return;
        }
        
        edge = current_edge;
          
        // Add new triangles.
        ggo::delaunay_triangle<T> triangle1(edge1._v1, edge1._v2, new_vertex);
        ggo::delaunay_triangle<T> triangle2(edge2._v1, edge2._v2, new_vertex);
        GGO_ASSERT(find_triangle(triangles, triangle1) == false);
        GGO_ASSERT(find_triangle(triangles, triangle2) == false);
        new_triangles.push_back(triangle1);
        new_triangles.push_back(triangle2);

        // Remove current triangle.
        old_triangles.push_back(*it_triangle);
      }
    }
    
    if (new_triangles.empty() == false && old_triangles.empty() == false)
    {
      GGO_ASSERT((new_triangles.size() == 2) || (new_triangles.size() == 4));
      GGO_ASSERT((old_triangles.size() == 1) || (old_triangles.size() == 2));
      
      for (it_triangle = new_triangles.begin(); it_triangle != new_triangles.end(); ++it_triangle)
      {
        triangles.push_back(*it_triangle);
      }
      
      for (it_triangle = old_triangles.begin(); it_triangle != old_triangles.end(); ++it_triangle)
      {
        remove_triangle(triangles, *it_triangle);
      }
      
      vertices.push_back(new_vertex);
      
      check_integrity(triangles);
      
      return;
    }
    
    // Last pass to seek for the the triangle in which the new vertex is.
    for (it_triangle = triangles.begin(); it_triangle != triangles.end(); ++it_triangle)
    {
      ggo::polygon2d<T> triangle;
      const ggo::pos2<T> * v1 = it_triangle->_v1;
      const ggo::pos2<T> * v2 = it_triangle->_v2;
      const ggo::pos2<T> * v3 = it_triangle->_v3;
      triangle.add_point(*v1);
      triangle.add_point(*v2);
      triangle.add_point(*v3);

      if (triangle.is_point_inside(new_vertex->template get<0>(), new_vertex->template get<1>()) == true)
      {
        // Add new triangles.
        ggo::delaunay_triangle<T> triangle1(it_triangle->_v1, it_triangle->_v2, new_vertex);
        ggo::delaunay_triangle<T> triangle2(it_triangle->_v2, it_triangle->_v3, new_vertex);
        ggo::delaunay_triangle<T> triangle3(it_triangle->_v3, it_triangle->_v1, new_vertex);
        GGO_ASSERT(find_triangle(triangles, triangle1) == false);
        GGO_ASSERT(find_triangle(triangles, triangle2) == false);
        GGO_ASSERT(find_triangle(triangles, triangle3) == false);
        triangles.push_back(triangle1);
        triangles.push_back(triangle2);
        triangles.push_back(triangle3);

        // Remove current triangle.
        triangles.erase(it_triangle);
        
        check_integrity(triangles);
        
        // Add new vertex.
        vertices.push_back(new_vertex);

        return;
      }
    }
    
    GGO_FAIL();
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  std::list<ggo::delaunay_triangle<T>> delaunay_triangulation_T(const std::vector<ggo::pos2<T>> & input_points)
  {
    if (input_points.size() < 2)
    {
      return std::list<ggo::delaunay_triangle<T>>();
    }
    
    std::list<ggo::delaunay_triangle<T>> output_triangles;
    std::vector<const ggo::pos2<T> *> vertices;
    
    // Add bounding triangles so that they overlap all the points.
    T min_x = input_points[0].template get<0>();
    T min_y = input_points[0].template get<1>();
    T max_x = input_points[0].template get<0>();
    T max_y = input_points[0].template get<1>();
    for (size_t i = 1; i < input_points.size(); ++i)
    {
      min_x = std::min(min_x, input_points[i].template get<0>());
      min_y = std::min(min_y, input_points[i].template get<1>());
      max_x = std::max(max_x, input_points[i].template get<0>());
      max_y = std::max(max_y, input_points[i].template get<1>());
    }
    T range_x = max_x - min_x;
    T range_y = max_y - min_y;
    ggo::pos2<T> v1(min_x - range_x, min_y - range_y);
    ggo::pos2<T> v2(min_x - range_x, max_y + range_y);
    ggo::pos2<T> v3(max_x + range_x, max_y + range_y);
    ggo::pos2<T> v4(max_x + range_x, min_y - range_y);
    
    vertices.push_back(&v1);
    vertices.push_back(&v2);
    vertices.push_back(&v3);
    vertices.push_back(&v4);
    
    output_triangles.push_back(ggo::delaunay_triangle<T>(&v1, &v2, &v3));
    output_triangles.push_back(ggo::delaunay_triangle<T>(&v1, &v3, &v4));

    // First we build a consistant triangle list.
    for (size_t i = 0; i < input_points.size(); ++i)
    {
      const ggo::pos2<T> & v = input_points[i];
      
      add_new_vertex(&v, output_triangles, vertices);
    }

    // Then we triangulate.
    int watch_dog = static_cast<int>(output_triangles.size() * output_triangles.size());
    for (int i = 0; i < watch_dog; ++i)
    {
      if (update_triangulation(output_triangles) == false)
      {
        break;
      }
    }
    
    // We need to remove all triangle with vertices from the initial bounding triangles.
    typename std::list<ggo::delaunay_triangle<T>>::iterator it;
    it = output_triangles.begin();
    while (it != output_triangles.end())
    {
      ggo::delaunay_triangle<T> & triangle = *it;

      if ((triangle.find(&v1) == true) || (triangle.find(&v2) == true) || (triangle.find(&v3) == true) || (triangle.find(&v4) == true))
      {
        typename std::list<ggo::delaunay_triangle<T>>::iterator it_tmp = it++;

        output_triangles.erase(it_tmp);
      }
      else 
      {
        it++;
      }
    }
    
    return output_triangles;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  std::list<delaunay_triangle<double>> delaunay_triangulation(const std::vector<ggo::pos2d> & input_points)
  {
    return delaunay_triangulation_T<double>(input_points);
  }

  //////////////////////////////////////////////////////////////
  std::list<delaunay_triangle<float>> delaunay_triangulation(const std::vector<ggo::pos2f> & input_points)
  {
    return delaunay_triangulation_T<float>(input_points);
  }
}
