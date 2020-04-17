#pragma once

#include <symbolic/ggo_expression.h>
#include <kernel/ggo_kernel.h>
#include <memory>

namespace ggo
{
  class exp_operation : public expression
  {
  public:

    exp_operation(std::shared_ptr<const expression> expression) : exp_operation(expression, ggo::e_d) { }
    exp_operation(std::shared_ptr<const expression> expression, double base) : _expression(expression), _base(base) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;

    std::string                       tree_repr(const std::string& indent) const override;
    std::string                       flat_repr() const override;

  private:

    std::shared_ptr<const expression> _expression;
    double                            _base;
  };

  std::shared_ptr<const expression> exp(std::shared_ptr<const expression> expression);
  std::shared_ptr<const expression> exp(std::shared_ptr<const expression> expression, double base);
}
