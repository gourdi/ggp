#include "ggo_morphology_processing.h"
#include <kernel/ggo_string_helpers.h>
#include <2d/ggo_image.h>
#include <2d/processing/ggo_morphology.h>
#include <vivelle/ggo_command.h>
#include <vivelle/ggo_parsing.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  ggo::image morphology_processing::process(ggo::image img, const parameters & params) const
  {
    auto rect = params.get({ "rect" });
    if (rect)
    {
      return apply_rectangle_operation(std::move(img), *rect);
    }

    auto disc = params.get<float>({ "disc", "radius" });
    if (disc)
    {
      return apply_disc_operation(std::move(img), *disc);
    }

    throw std::runtime_error("invalid erosion parameter");

    return img;
  }

  /////////////////////////////////////////////////////////////////////
  ggo::image morphology_processing::apply_rectangle_operation(ggo::image img, const std::string & params) const
  {
    ggo::image output(img.size(), img.pixel_type(), img.memory_lines_order());

    auto wh = ggo::split(params, ',');
    if (wh.size() != 2)
    {
      throw std::runtime_error("expecting 2 parameters for erosion");
    }
    int w = to<int>(wh[0]);
    int h = to<int>(wh[1]);

    switch (_type)
    {
    case type::erosion:
      ggo::erosion_rectangle(img, output, w, h);
      break;
    case type::dilatation:
      ggo::dilatation_rectangle(img, output, w, h);
      break;
    default:
      throw std::runtime_error("unexpected morphology type");
    }

    return output;
  }

  /////////////////////////////////////////////////////////////////////
  ggo::image morphology_processing::apply_disc_operation(ggo::image img, float radius) const
  {
    ggo::image output(img.size(), img.pixel_type(), img.memory_lines_order());

    switch (_type)
    {
    case type::erosion:
      ggo::erosion_disc(img, output, radius);
      break;
    case type::dilatation:
      ggo::erosion_disc(img, output, radius);
      break;
    default:
      throw std::runtime_error("unexpected morphology type");
    }

    return output;
  }
}