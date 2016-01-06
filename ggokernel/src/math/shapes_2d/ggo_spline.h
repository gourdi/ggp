//////////////////////////////////////////////////////////////////
//
// WARNING : NEVER DIRECTLY INCLUDE THIS FILE!
// INCLUDE 'ggo_math.h' INSTEAD.
//
//////////////////////////////////////////////////////////////////
//
// This class implements uniform cubic B-spline.
// http://en.wikipedia.org/wiki/B-spline#Uniform_cubic_B-splines
//
//////////////////////////////////////////////////////////////////

namespace ggo
{
  template <typename T>
  class spline : public movable_shape2d_abc<T>,
                 public rotatable_shape2d_abc<T>
  {
  public:

                              spline() {}
      
    void	                	  add_control_point(const ggo::set2<T> & point);
    void	                	  clear_control_points();
    
    std::vector<ggo::set2<T>>	evaluate(int steps) const;

    void	                	  move(T dx, T dy) override;
    void	                	  rotate(T angle, const ggo::set2<T> & center) override;

  private:

    static	ggo::set2<T>      eval(const ggo::set2<T> & pp, const ggo::set2<T> & pc, const ggo::set2<T> & pn, const ggo::set2<T> & pnn, T t);

  private:

    std::vector<ggo::set2<T>>	_control_points;
  };
}
