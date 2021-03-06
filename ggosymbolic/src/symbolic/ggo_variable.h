#pragma once

#include <symbolic/ggo_expression.h>

namespace ggo
{
  class variable : public expression
  {
  public:

    variable(std::string name) : _name(name) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;

    std::optional<std::string>        get_variable() const override { return _name; }

    std::string                       tree_repr(const std::string& indent) const override;
    std::string                       flat_repr() const override;

  private:

    const std::string _name;
  };

  std::shared_ptr<expression> make_variable(const std::string & v);
}

std::shared_ptr<const ggo::expression> operator"" _symb(const char * v, std::size_t l);
