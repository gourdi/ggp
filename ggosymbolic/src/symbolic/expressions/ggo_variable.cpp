#include <symbolic/expressions/ggo_variable.h>
#include <symbolic/expressions/ggo_constant.h>
#include <stdexcept>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double variable::eval(const std::map<std::string, double> & env) const
  {
    auto it = env.find(_name);
    if (it == env.end())
    {
      throw std::runtime_error("missing variable '" + _name + "' in environment");
    }

    return it->second;
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> variable::derivate(const std::string & var) const
  {
    return std::make_shared<ggo::constant>(var == _name ? 1. : 0.);
  }

  /////////////////////////////////////////////////////////////////////
  std::string variable::tree_repr(const std::string & indent) const
  {
    return indent + _name;
  }

  /////////////////////////////////////////////////////////////////////
  std::string variable::flat_repr() const
  {
    return _name;
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<expression> make_variable(const std::string& v)
  {
    return std::make_shared<ggo::variable>(v);
  }
}

/////////////////////////////////////////////////////////////////////
std::shared_ptr<const ggo::expression> operator"" _symb(const char* v, std::size_t l)
{
  return ggo::make_variable(v);
}
