#include <symbolic/expressions/ggo_exp_operation.h>
#include <symbolic/expressions/ggo_log_operation.h>
#include <symbolic/expressions/ggo_binary_operation.h>
#include <symbolic/expressions/ggo_constant.h>
#include <symbolic/expressions/ggo_variable.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double exp_operation::eval(const std::map<std::string, double>& env) const
  {
    if (_base == e_d)
    {
      return std::exp(_expression->eval(env));
    }
    else
    {
      return std::pow(_base, _expression->eval(env));
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> exp_operation::derivate(const std::string& var) const
  {
    if (_base == e_d)
    {
      return ggo::exp(_expression) * _expression->derivate(var);
    }
    else
    {
      return ggo::exp(_expression, _base) * _expression->derivate(var) * ggo::log(_base);
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::string exp_operation::tree_repr(const std::string& indent) const
  {
    return (_base == e_d ? "e" : constant_to_str(_base)) + "^\n" + _expression->tree_repr(indent + "  ");
  }

  /////////////////////////////////////////////////////////////////////
  std::string exp_operation::flat_repr() const
  {
    return (_base == e_d ? "e" : constant_to_str(_base)) + "^(" +_expression->flat_repr() + ")";
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> exp(std::shared_ptr<const expression> expression)
  {
    return std::make_shared<exp_operation>(expression);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> exp(std::shared_ptr<const expression> expression, double base)
  {
    return std::make_shared<exp_operation>(expression, base);
  }
}
