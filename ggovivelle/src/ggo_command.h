#ifndef __GGO_COMMAND__
#define __GGO_COMMAND__

#include <string>
#include <map>
#include <optional>
#include <ggo_vec.h>

namespace ggo
{
  class parameters
  {
  public:

    parameters(const std::string & txt);

    bool                        has_key(const std::string & key) const { return _parameters.find(key) != _parameters.end(); }

    std::optional<std::string>  operator[](const std::string & key) const { auto it = _parameters.find(key); if (it == _parameters.end()) return {}; else return it->second; }

    bool                        empty() const { return _parameters.empty(); }
    size_t                      size() const { return _parameters.size(); }

  private:

    std::map<std::string, std::string> _parameters;
  };

  class command
  {
  public:

    command(const std::string & txt, bool filename_command = false);

    const std::string &         name() const { return _name; }
    const ggo::parameters &     parameters() const { return _parameters; }

    bool                        has_key(const std::string & key) const { return _parameters.has_key(key); }

    std::optional<std::string>  operator[](const std::string & key) { return _parameters[key]; }

    bool                        empty() const { return _parameters.empty(); }
    size_t                      size() const { return _parameters.size(); }

  private:

    std::string     _name;
    ggo::parameters _parameters;
  };

  int parse_margin(const parameters & params, int image_size, int content_size, const std::string & key);
}

#endif
