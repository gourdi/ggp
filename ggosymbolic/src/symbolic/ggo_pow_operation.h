#pragma once

#include <symbolic/ggo_expression.h>
#include <memory>

namespace ggo
{
  class pow_operation : public expression
  {
  public:

    pow_operation(std::shared_ptr<const expression> expression, double exponant) : _expression(expression), _exponant(exponant) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;

    std::string                       tree_repr(const std::string& indent) const override;
    std::string                       flat_repr() const override;

  private:

    std::shared_ptr<const expression> _expression;
    double                            _exponant;
  };

  std::shared_ptr<const expression> pow(std::shared_ptr<const expression> expression, double exponant);
}