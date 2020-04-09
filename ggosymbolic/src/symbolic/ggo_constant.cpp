#include <symbolic/ggo_constant.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double constant::eval(const std::map<std::string, double>& env) const
  {
    return _c;
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> constant::derivate(const std::string& var) const
  {
    return std::make_shared<ggo::constant>(0.);
  }

  /////////////////////////////////////////////////////////////////////
  std::string constant::tree_repr(const std::string& indent) const
  {
    return indent + flat_repr();
  }

  /////////////////////////////////////////////////////////////////////
  std::string constant::flat_repr() const
  {
    return constant_to_str(_c);
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<expression> make_constant(double c)
  {
    return std::make_shared<ggo::constant>(c);
  }

  /////////////////////////////////////////////////////////////////////
  std::string constant_to_str(double c)
  {
    std::string s = std::to_string(c);

    // Trim zeros and dot in case of integer value.
    s.erase(s.find_last_not_of("0") + 1);
    s.erase(s.find_last_not_of(".") + 1);

    return s;
  }
}

/////////////////////////////////////////////////////////////////////
std::shared_ptr<const ggo::expression> operator"" _symb(long double c)
{
  return ggo::make_constant(c);
}
