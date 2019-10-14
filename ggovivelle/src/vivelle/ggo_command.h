#ifndef __GGO_COMMAND__
#define __GGO_COMMAND__

#include <string>
#include <map>
#include <optional>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_vec.h>
#include <kernel/ggo_size.h>
#include <kernel/ggo_string_helpers.h>

namespace ggo
{
  class parameters
  {
  public:

                                parameters(const std::string & txt);

    template <typename data_t = std::string>  
    std::optional<data_t>       get(const std::initializer_list<std::string> & keys) const
    {
      std::optional<std::string> res;

      for (const auto & key : keys)
      {
        auto it = _parameters.find(key);
        if (it != _parameters.end())
        {
          if (res.has_value() == true)
          {
            throw std::runtime_error("multiple keys");
          }

          res = std::optional<std::string>(it->second);
        }
      }

      if constexpr(std::is_same<data_t, std::string>::value == true)
      {
        return res;
      }

      if (!res)
      {
        return {};
      }
      else
      {
        return to<data_t>(*res);
      }
    }

    bool                        empty() const { return _parameters.empty(); }
    size_t                      size() const { return _parameters.size(); }

  private:

    std::map<std::string, std::string> _parameters;
  };

  class command
  {
  public:

    enum class policy
    {
      filename = 1
    };

                                command(const std::string & txt, bit_mask<policy> policies = {});

    const std::string &         name() const { return _name; }
    const ggo::parameters &     parameters() const { return _parameters; }

  private:

    std::string     _name;
    ggo::parameters _parameters;
  };
}

#endif
