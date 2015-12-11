#ifndef __GGO_DELAUNAY_TRIANGULATION__
#define __GGO_DELAUNAY_TRIANGULATION__

#include <ggo_set2.h>
#include <list>
#include <vector>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class delaunay_triangle
  {
  public:
     
          delaunay_triangle(const set2<T> * v1, const set2<T> * v2, const set2<T> * v3) : _v1(v1), _v2(v2), _v3(v3) {};
        
    bool	find(const set2<T> * v) const { return _v1 == v || _v2 == v || _v3 == v; };

    bool	is_valid() const;
    bool	compare(const delaunay_triangle & other) const;

  public:

    const set2<T> * _v1;
    const set2<T> * _v2;
    const set2<T> * _v3;
  };

  //////////////////////////////////////////////////////////////
  std::list<delaunay_triangle<double>> delaunay_triangulation(const std::vector<ggo::point2d_double> & input_points);
  std::list<delaunay_triangle<float>>  delaunay_triangulation(const std::vector<ggo::point2d_float> & input_points);
}
  
#endif
