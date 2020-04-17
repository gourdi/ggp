#pragma once

#include <string>
#include <map>
#include <memory>
#include <optional>

namespace ggo
{
  class expression
  {
  public:

    virtual double                            eval(const std::map<std::string, double>& env) const = 0;
    virtual std::shared_ptr<const expression> derivate(const std::string & var) const = 0;

    virtual std::optional<double>             get_constant() const { return {}; }
    virtual std::optional<std::string>        get_variable() const { return {}; }

    virtual std::string tree_repr(const std::string& indent) const = 0;
    virtual std::string flat_repr() const = 0;
  };
}