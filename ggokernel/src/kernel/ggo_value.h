#ifndef __GGO_VALUE__
#define __GGO_VALUE__

#include <variant>
#include <kernel/ggo_vec2.h>
#include <kernel/ggo_vec3.h>

namespace ggo
{
  class value final
  {
  public:

    template <typename data_t>
    value(data_t v) : _variant(v) {}

    template <typename data_t>
    const data_t & to() const { return std::get<data_t>(_variant); }

    template <typename func_int_t, typename func_float_t, typename func_vec2_f_t, typename func_vec3f_f_t>
    void visit(func_int_t && f_int, func_float_t && f_float, func_vec2_f_t && f_vec2_f, func_vec3f_f_t && f_vec3_f) const
    {
      switch (_variant.index())
      {
      case 0:
        f_int(std::get<int>(_variant));
        break;
      case 1:
        f_float(std::get<float>(_variant));
        break;
      case 2:
        f_vec2_f(std::get<vec2_f>(_variant));
        break;
      case 3:
        f_vec3_f(std::get<vec3_f>(_variant));
        break;
      }
    }

  private:

    std::variant<int, float, vec2_f, vec3_f> _variant;
  };
}

#endif
