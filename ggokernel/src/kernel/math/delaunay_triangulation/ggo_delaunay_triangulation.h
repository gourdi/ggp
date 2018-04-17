#ifndef __GGO_DELAUNAY_TRIANGULATION__
#define __GGO_DELAUNAY_TRIANGULATION__

#include <kernel/ggo_vec.h>
#include <list>
#include <vector>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class delaunay_triangle
  {
  public:
     
          delaunay_triangle(const pos2<T> * v1, const pos2<T> * v2, const pos2<T> * v3) : _v1(v1), _v2(v2), _v3(v3) {};
        
    bool	find(const pos2<T> * v) const { return _v1 == v || _v2 == v || _v3 == v; };

    bool	is_valid() const;
    bool	compare(const delaunay_triangle & other) const;

  public:

    const pos2<T> * _v1;
    const pos2<T> * _v2;
    const pos2<T> * _v3;
  };

  //////////////////////////////////////////////////////////////
  std::list<delaunay_triangle<double>> delaunay_triangulation(const std::vector<ggo::pos2d> & input_points);
  std::list<delaunay_triangle<float>>  delaunay_triangulation(const std::vector<ggo::pos2f> & input_points);
}
  
#endif
