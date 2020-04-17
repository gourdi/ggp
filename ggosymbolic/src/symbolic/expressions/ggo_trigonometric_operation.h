#pragma once

#include <symbolic/expressions/ggo_expression.h>

namespace ggo
{
  class trigonometric_operation : public expression
  {
  public:

    enum type
    {
      cos,
      sin,
      tan
    };

    trigonometric_operation(type t, std::shared_ptr<const expression> expression) : _type(t), _expression(expression) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;

    std::string                       tree_repr(const std::string& indent) const override;
    std::string                       flat_repr() const override;

  private:

    type                              _type;
    std::shared_ptr<const expression> _expression;
  };

  std::shared_ptr<const expression> cos(std::shared_ptr<const expression> expression);
  std::shared_ptr<const expression> sin(std::shared_ptr<const expression> expression);
  std::shared_ptr<const expression> tan(std::shared_ptr<const expression> expression);
}
