#ifndef __GGO_VALUE__
#define __GGO_VALUE__

#include <variant>

namespace ggo
{
  class value
  {
  public:

    template <typename data_t>
    value(data_t v) { _variant = v; }

    template <typename data_t>
    data_t to() const { return std::get<data_t>(_variant); }

    /*template <typename data_t>
    const data_t * to_array_of() const;
*/
  private:

    std::variant<int, float> _variant;
  };
}

#endif
