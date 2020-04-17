#include <symbolic/ggo_binary_operation.h>
#include <symbolic/ggo_constant.h>
#include <symbolic/ggo_pow_operation.h>
#include <stdexcept>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  double binary_operation::eval(const std::map<std::string, double>& env) const
  {
    switch (_type)
    {
    case add:
      return _left->eval(env) + _right->eval(env);
    case sub:
      return _left->eval(env) - _right->eval(env);
    case mul:
      return _left->eval(env) * _right->eval(env);
    case div:
      return _left->eval(env) / _right->eval(env);
    default:
      throw std::runtime_error("unexpected binary operation type");
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> binary_operation::derivate_add(const std::string& var) const
  {
    return _left->derivate(var) + _right->derivate(var);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> binary_operation::derivate_sub(const std::string& var) const
  {
    return _left->derivate(var) - _right->derivate(var);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> binary_operation::derivate_mul(const std::string& var) const
  {
    return _left->derivate(var) * _right + _left * _right->derivate(var);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> binary_operation::derivate_div(const std::string& var) const
  {
    auto num = _left->derivate(var) * _right - _left * _right->derivate(var);
    auto den = pow(_right, 2.);
    return num / den;
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const expression> binary_operation::derivate(const std::string& var) const
  {
    switch (_type)
    {
    case add:
      return derivate_add(var);
    case sub:
      return derivate_sub(var);
    case mul:
      return derivate_mul(var);
    case div:
      return derivate_div(var);
    default:
      throw std::runtime_error("unexpected binary operation type");
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::string binary_operation::tree_repr(const std::string & indent) const
  {
    std::string r(indent);

    switch (_type)
    {
    case add:
      r += "+";
      break;
    case sub:
      r += "-";
      break;
    case mul:
      r += "*";
      break;
    case div:
      r += "/";
      break;
    default:
      throw std::runtime_error("unexpected binary operation type");
      break;
    }

    return r + "\n" + _left->tree_repr(indent + "  ") + "\n" + _right->tree_repr(indent + "  ") + "\n";
  }

  /////////////////////////////////////////////////////////////////////
  std::string binary_operation::flat_repr() const
  {
    std::string r("(" + _left->flat_repr());

    switch (_type)
    {
    case add:
      r += "+";
      break;
    case sub:
      r += "-";
      break;
    case mul:
      r += "*";
      break;
    case div:
      r += "/";
      break;
    default:
      throw std::runtime_error("unexpected binary operation type");
      break;
    }

    return r + _right->flat_repr() + ")";
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const ggo::expression> operator+(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right)
  {
    auto left_constant = left->get_constant();
    auto right_constant = right->get_constant();

    if (left_constant && right_constant)
    {
      return std::make_shared<ggo::constant>(*left_constant + *right_constant);
    }

    if (left_constant && *left_constant == 0.f)
    {
      return right;
    }

    if (right_constant && *right_constant == 0.f)
    {
      return left;
    }

    return std::make_shared<ggo::binary_operation>(ggo::binary_operation::add, left, right);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const ggo::expression> operator-(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right)
  {
    auto left_constant = left->get_constant();
    auto right_constant = right->get_constant();

    if (left_constant && right_constant)
    {
      return std::make_shared<ggo::constant>(*left_constant - *right_constant);
    }

    if (right_constant && *right_constant == 0.)
    {
      return left;
    }

    return std::make_shared<ggo::binary_operation>(ggo::binary_operation::sub, left, right);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const ggo::expression> operator*(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right)
  {
    auto left_constant = left->get_constant();
    auto right_constant = right->get_constant();

    if (left_constant && right_constant)
    {
      return std::make_shared<ggo::constant>(*left_constant * *right_constant);
    }

    if (left_constant && *left_constant == 0.)
    {
      return std::make_shared<ggo::constant>(0.);
    }

    if (right_constant && *right_constant == 0.)
    {
      return std::make_shared<ggo::constant>(0.);
    }

    if (left_constant && *left_constant == 1.)
    {
      return right;
    }

    if (right_constant && *right_constant == 1.)
    {
      return left;
    }

    return std::make_shared<ggo::binary_operation>(ggo::binary_operation::mul, left, right);
  }

  /////////////////////////////////////////////////////////////////////
  std::shared_ptr<const ggo::expression> operator/(std::shared_ptr<const ggo::expression> left, std::shared_ptr<const ggo::expression> right)
  {
    auto left_constant = left->get_constant();
    auto right_constant = right->get_constant();

    if (left_constant && right_constant)
    {
      return std::make_shared<ggo::constant>(*left_constant / *right_constant);
    }

    if (left_constant && *left_constant == 0.)
    {
      return std::make_shared<ggo::constant>(0.);
    }

    if (right_constant && *right_constant == 1.)
    {
      return left;
    }

    return std::make_shared<ggo::binary_operation>(ggo::binary_operation::div, left, right);
  }
}
