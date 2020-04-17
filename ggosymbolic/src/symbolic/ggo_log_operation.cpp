#include <symbolic/ggo_log_operation.h>
#include <symbolic/ggo_constant.h>
#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_variable.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double log_operation::eval(const std::map<std::string, double>& env) const
  {
    if (_base == e_d)
    {
      return std::log(_expression->eval(env));
    }
    else
    {
      return std::log(_expression->eval(env)) / std::log(_base);
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> log_operation::derivate(const std::string& var) const
  {
    if (_base == e_d)
    {
      return _expression->derivate(var) / _expression;
    }
    else
    {
      return _expression->derivate(var) / (ggo::log(make_constant(_base)) * _expression);
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::string log_operation::tree_repr(const std::string& indent) const
  {
    std::string repr = indent + "log";

    if (_base == e_d)
    {
      repr += " base " + constant_to_str(_base);
    }

    repr += "\n" + _expression->tree_repr(indent + "  ");

    return repr;
  }

  /////////////////////////////////////////////////////////////////////
  std::string log_operation::flat_repr() const
  {
    if (_base == e_d)
    {
      return "log(" + _expression->flat_repr() + ")";
    }
    else
    {
      return "log_" + constant_to_str(_base) + "(" + _expression->flat_repr() + ")";
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> log(std::shared_ptr<const expression> expression)
  {
    return std::make_shared<log_operation>(expression);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> log(double d)
  {
    return std::make_shared<log_operation>(make_constant(d));
  }
}
