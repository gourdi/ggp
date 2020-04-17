#pragma once

#include <symbolic/ggo_expression.h>

namespace ggo
{
  class binary_operation : public expression
  {
  public:

    enum type
    {
      add,
      sub,
      mul,
      div
    };

    binary_operation(type t, std::shared_ptr<const expression> left, std::shared_ptr<const expression> right) : _type(t), _left(left), _right(right) { }

    double                            eval(const std::map<std::string, double>& env) const override;
    std::shared_ptr<const expression> derivate(const std::string& var) const override;

    std::string                       tree_repr(const std::string& indent) const override;
    std::string                       flat_repr() const override;

  private:

    std::shared_ptr<const expression> derivate_add(const std::string & var) const;
    std::shared_ptr<const expression> derivate_sub(const std::string & var) const;
    std::shared_ptr<const expression> derivate_mul(const std::string & var) const;
    std::shared_ptr<const expression> derivate_div(const std::string & var) const;
                                                                     
  private:

    type                              _type;
    std::shared_ptr<const expression> _left;
    std::shared_ptr<const expression> _right;
  };

  std::shared_ptr<const ggo::expression> operator+(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right);
  std::shared_ptr<const ggo::expression> operator-(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right);
  std::shared_ptr<const ggo::expression> operator*(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right);
  std::shared_ptr<const ggo::expression> operator/(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right);
}

