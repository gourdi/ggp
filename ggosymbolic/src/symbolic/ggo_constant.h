#pragma once

#include <symbolic/ggo_expression.h>

namespace ggo
{
  class constant : public expression
  {
  public:

    constant(double c) : _c(c) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;
        
    bool                              is_constant(double c) const override { return c == _c; }

    std::string                       tree_repr(const std::string & indent) const override;
    std::string                       flat_repr() const override;

  private:

    const double _c;
  };

  std::shared_ptr<expression> make_constant(double c);
  std::string                 constant_to_str(double c);
}

std::shared_ptr<const ggo::expression> operator"" _symb(long double c);
