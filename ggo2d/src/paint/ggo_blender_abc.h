#ifndef __GGO_BLENDER_ABC__
#define __GGO_BLENDER_ABC__

#include <ggo_color.h>

namespace ggo
{
  template <typename color_t, typename real_t>
  struct blender_abc
  {
    virtual color_t blend(const color_t & bkdg_color, real_t intensity, const color_t & paint_color) const = 0;
  };
}

namespace ggo
{
  template <typename color_t>
  struct opaque_blender
  {
    color_t blend(const color_t & bkdg_color, const color_t & paint_color) const
    {
      return paint_color;
    }
  };
}

namespace ggo
{
  template <typename color_t, typename real_t>
  struct alpha_blender : public blender_abc<color_t, real_t>
  {
    color_t blend(const color_t & bkdg_color, real_t intensity, const color_t & paint_color) const override
    {
      return intensity * paint_color + (1 - intensity) * bkdg_color;
    }
  };
}

namespace ggo
{
  template <typename color_t, typename real_t>
  struct additive_blender : public blender_abc<color_t, real_t>
  {
    color_t blend(const color_t & bkdg_color, real_t intensity, const color_t & paint_color) const override
    {
       return intensity * paint_color + bkdg_color;
    }
  };
}

#endif

