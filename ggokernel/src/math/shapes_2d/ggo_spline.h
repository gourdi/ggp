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
  template <typename data_t>
  class spline final : public movable_shape2d_abc<data_t>,
                       public rotatable_shape2d_abc<data_t>
  {
  public:

                                    spline() {}
      
    void	                	        add_control_point(const ggo::pos2<data_t> & point);
    void	                	        clear_control_points();
    
    std::vector<ggo::pos2<data_t>>	evaluate(int steps) const;

    void	                	        move(data_t dx, data_t dy) override;
    void	                	        rotate(data_t angle, const ggo::pos2<data_t> & center) override;

  private:

    static	ggo::pos2<data_t>      eval(const ggo::pos2<data_t> & pp, const ggo::pos2<data_t> & pc, const ggo::pos2<data_t> & pn, const ggo::pos2<data_t> & pnn, data_t t);

  private:

    std::vector<ggo::pos2<data_t>>	_control_points;
  };
}
