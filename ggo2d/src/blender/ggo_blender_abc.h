#ifndef __GGO_BLENDER_ABC__
#define __GGO_BLENDER_ABC__

#include <ggo_color.h>

namespace ggo
{
  template <typename T>
  struct blender_abc
  {
    virtual T blend(const T & bkdg_color, float opacity, const T & paint_color) const = 0;
  };

  using gray_blender_abc = blender_abc<float>;
  using rgb_blender_abc = blender_abc<ggo::color>;
}

namespace ggo
{
  template <typename T>
  struct alpha_blender : public blender_abc<T>
  {
    T blend(const T & bkdg_color, float opacity, const T & paint_color) const override
    {
      return opacity * paint_color + (1 - opacity) * bkdg_color;
    }
  };

  using gray_alpha_blender = alpha_blender<float>;
  using rgb_alpha_blender = alpha_blender<ggo::color>;
}

namespace ggo
{
  template <typename T>
  struct additive_blender : public blender_abc<T>
  {
    T blend(const T & bkdg_color, float opacity, const T & paint_color) const override
    {
       return opacity * paint_color + bkdg_color;
    }
  };

  using gray_additive_blender = additive_blender<float>;
  using rgb_additive_blender = additive_blender<ggo::color>;
}

#endif

