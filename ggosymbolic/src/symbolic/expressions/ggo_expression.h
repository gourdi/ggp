#pragma once

#include <string>
#include <map>
#include <memory>

namespace ggo
{
  class expression
  {
  public:

    virtual double                            eval(const std::map<std::string, double>& env) const = 0;
    virtual std::shared_ptr<const expression> derivate(const std::string & var) const = 0;

    virtual bool                              is_constant(double c) const = 0;

    virtual std::string tree_repr(const std::string& indent) const = 0;
    virtual std::string flat_repr() const = 0;
  };
}