#include <symbolic/ggo_pow_operation.h>
#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_constant.h>
#include <cmath>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double pow_operation::eval(const std::map<std::string, double>& env) const
  {
    return std::pow(_expression->eval(env), _exponant);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> pow_operation::derivate(const std::string& var) const
  {
    return make_constant(_exponant) * pow(_expression, _exponant - 1);
  }

  /////////////////////////////////////////////////////////////////////
  std::string pow_operation::tree_repr(const std::string& indent) const
  {
    return indent + "^" + constant_to_str(_exponant) + "\n" + _expression->tree_repr(indent + "  ");
  }

  /////////////////////////////////////////////////////////////////////
  std::string pow_operation::flat_repr() const
  {
    return _expression->flat_repr() + "^" + constant_to_str(_exponant);
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> pow(std::shared_ptr<const expression> expression, double exponant)
  {
    if (exponant == 0)
    {
      return 1._symb;
    }

    if (exponant == 1)
    {
      return expression;
    }

    if (exponant == -1)
    {
      return 1._symb / expression;
    }

    return std::make_shared<pow_operation>(expression, exponant);
  }
}
