#include <symbolic/expressions/ggo_trigonometric_operation.h>
#include <symbolic/expressions/ggo_binary_operation.h>
#include <symbolic/expressions/ggo_constant.h>
#include <symbolic/expressions/ggo_pow_operation.h>
#include <cmath>
#include <stdexcept>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double trigonometric_operation::eval(const std::map<std::string, double>& env) const
  {
    switch (_type)
    {
    case type::cos:
      return std::cos(_expression->eval(env));
    case type::sin:
      return std::sin(_expression->eval(env));
    case type::tan:
      return std::tan(_expression->eval(env));
    default:
      throw std::runtime_error("unexpected trigonometric type");
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> trigonometric_operation::derivate(const std::string& var) const
  {
    switch (_type)
    {
    case type::cos:
      return ggo::cos(_expression) * _expression->derivate(var);
    case type::sin:
      return make_constant(0.) - ggo::sin(_expression) * _expression->derivate(var);
    case type::tan:
      return (1._symb + ggo::pow(ggo::tan(_expression), 2.)) * _expression->derivate(var);
    default:
      throw std::runtime_error("unexpected trigonometric type");
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::string trigonometric_operation::tree_repr(const std::string& indent) const
  {
    std::string repr(indent);

    switch (_type)
    {
    case type::cos:
      repr += "cos";
      break;
    case type::sin:
      repr += "sin";
      break;
    case type::tan:
      repr += "tan";
      break;
    default:
      throw std::runtime_error("unexpected trigonometric type");
    }

    return repr + "\n" + _expression->tree_repr(indent + "  ");
  }

  /////////////////////////////////////////////////////////////////////
  std::string trigonometric_operation::flat_repr() const
  {
    switch (_type)
    {
    case type::cos:
      return "cos(" + _expression->flat_repr() + ")";
    case type::sin:
      return "sin(" + _expression->flat_repr() + ")";
    case type::tan:
      return "tan(" + _expression->flat_repr() + ")";
    default:
      throw std::runtime_error("unexpected trigonometric type");
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> cos(std::shared_ptr<const expression> expression)
  {
    return std::make_shared<trigonometric_operation>(trigonometric_operation::cos, expression);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> sin(std::shared_ptr<const expression> expression)
  {
    return std::make_shared<trigonometric_operation>(trigonometric_operation::sin, expression);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> tan(std::shared_ptr<const expression> expression)
  {
    return std::make_shared<trigonometric_operation>(trigonometric_operation::tan, expression);
  }

}
