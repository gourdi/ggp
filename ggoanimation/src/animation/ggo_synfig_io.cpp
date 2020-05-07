#include <animation/ggo_synfig_io.h>
#include <kernel/math/interpolation/ggo_interpolation1d.h>
#include <2d/blend/ggo_alpha_blend.h>
#include <2d/brush/ggo_solid_color_brush.h>
#include <tinyxml2.h>


namespace ggo
{
  namespace
  {
    std::vector<keyframe<float>> read_float_animation(const tinyxml2::XMLElement* param)
    {
      std::vector<std::pair<float, float>> animation;

      if (auto value = param->FirstChildElement("real"); value != nullptr)
      {
        animation.push_back({ 0.f, value->FloatAttribute("value") });
      }
      else if (auto animated = param->FirstChildElement("animated"); animated != nullptr)
      {
        for (auto waypoint = animated->FirstChildElement("waypoint"); waypoint != nullptr; waypoint = waypoint->NextSiblingElement("waypoint"))
        {
          float value = waypoint->FirstChildElement("real")->FloatAttribute("value");
          float time = waypoint->FloatAttribute("time");
          animation.push_back({ time, value });
        }
      }

      return animation;
    }

    std::vector<ggo::keyframe<ggo::vec2_f>> read_vec2f_animation(const tinyxml2::XMLElement* param)
    {
      auto read_vector = [](const tinyxml2::XMLElement* vector)
      {
        float x = vector->FirstChildElement("x")->FloatText();
        float y = vector->FirstChildElement("y")->FloatText();
        return ggo::vec2_f(x, y);
      };

      std::vector<ggo::keyframe<ggo::vec2_f>> animation;

      if (auto vector = param->FirstChildElement("vector"); vector != nullptr)
      {
        animation.push_back({ 0.f, read_vector(vector) });
      }
      else if (auto animated = param->FirstChildElement("animated"); animated != nullptr)
      {
        for (auto waypoint = animated->FirstChildElement("waypoint"); waypoint != nullptr; waypoint = waypoint->NextSiblingElement("waypoint"))
        {
          ggo::vec2_f value = read_vector(waypoint->FirstChildElement("vector"));
          float time = waypoint->FloatAttribute("time");
          animation.push_back({ time, value });
        }
      }

      return animation;
    }

    std::vector<ggo::keyframe<ggo::rgba_32f>> read_color_animation(const tinyxml2::XMLElement* param)
    {
      auto read_color = [](const tinyxml2::XMLElement* vector)
      {
        float r = vector->FirstChildElement("r")->FloatText();
        float g = vector->FirstChildElement("g")->FloatText();
        float b = vector->FirstChildElement("b")->FloatText();
        float a = vector->FirstChildElement("a")->FloatText();
        return ggo::rgba_32f(r, g, b, a);
      };

      std::vector<ggo::keyframe<ggo::rgba_32f>> animation;

      if (auto color = param->FirstChildElement("color"); color != nullptr)
      {
        animation.push_back({ 0.f, read_color(color) });
      }
      else if (auto animated = param->FirstChildElement("animated"); animated != nullptr)
      {
        for (auto waypoint = animated->FirstChildElement("waypoint"); waypoint != nullptr; waypoint = waypoint->NextSiblingElement("waypoint"))
        {
          ggo::rgba_32f value = read_color(waypoint->FirstChildElement("color"));
          float time = waypoint->FloatAttribute("time");
          animation.push_back({ time, value });
        }
      }

      return animation;
    }

    std::unique_ptr<ggo::circle_animation> read_circle(const tinyxml2::XMLElement* layer)
    {
      auto animation = std::make_unique<ggo::circle_animation>();

      for (auto param = layer->FirstChildElement("param"); param != nullptr; param = param->NextSiblingElement("param"))
      {
        if (param->Attribute("name") == std::string("radius"))
        {
          animation->_radius = read_float_animation(param);
        }
        else if (param->Attribute("name") == std::string("origin"))
        {
          animation->_center = read_vec2f_animation(param);
        }
        else if (param->Attribute("name") == std::string("color"))
        {
          animation->_color = read_color_animation(param);
        }
      }

      return animation;
    }

    std::unique_ptr<ggo::rectangle_animation> read_rectangle(const tinyxml2::XMLElement* layer)
    {
      auto animation = std::make_unique<ggo::rectangle_animation>();

      for (auto param = layer->FirstChildElement("param"); param != nullptr; param = param->NextSiblingElement("param"))
      {
        if (param->Attribute("name") == std::string("point1"))
        {
          animation->_p1 = read_vec2f_animation(param);
        }
        else if (param->Attribute("name") == std::string("point2"))
        {
          animation->_p2 = read_vec2f_animation(param);
        }
        else if (param->Attribute("name") == std::string("color"))
        {
          animation->_color = read_color_animation(param);
        }
      }

      return animation;
    }
  }

  ggo::rect_f read_view(const tinyxml2::XMLElement* canvas)
  {
    auto coords = ggo::split(canvas->Attribute("view-box"), ' ');

    float left   = ggo::to<float>(coords[0]);
    float top    = ggo::to<float>(coords[1]);
    float right  = ggo::to<float>(coords[2]);
    float bottom = ggo::to<float>(coords[3]);

    return ggo::rect_f::from_left_right_bottom_top(left, right, bottom, top);
  }

  ggo::rgb_32f read_bkgd_color(const tinyxml2::XMLElement* canvas)
  {
    auto color = ggo::split(canvas->Attribute("bgcolor"), ' ');

    float r = ggo::to<float>(color[0]);
    float g = ggo::to<float>(color[1]);
    float b = ggo::to<float>(color[2]);

    return ggo::rgb_32f(r, g, b);
  }

  animation load_synfig_animation()
  {
    animation anim;

    tinyxml2::XMLDocument doc;
    doc.LoadFile("d:/test.sif");

    auto canvas = doc.FirstChildElement("canvas");

    anim._view = read_view(canvas);
    anim._bkgd_color = read_bkgd_color(canvas);

    for (auto layer = canvas->FirstChildElement("layer"); layer != nullptr; layer = layer->NextSiblingElement("layer"))
    {
      if (layer->Attribute("type") == std::string("circle"))
      {
        anim._layer_animations.push_back(read_circle(layer));
      }
      else if (layer->Attribute("type") == std::string("rectangle"))
      {
        anim._layer_animations.push_back(read_rectangle(layer));
      }
    }

    return anim;
  }

  std::shared_ptr<const layer<ggo::rgb_8u>> circle_animation::layer_at_time(float time, view_transform<float> transform) const
  {
    float radius = ggo::linear_interpolation1d(_radius.begin(), _radius.end(), time);
    ggo::pos2_f center = ggo::linear_interpolation1d(_center.begin(), _center.end(), time);
    ggo::rgba_32f color = ggo::linear_interpolation1d(_color.begin(), _color.end(), time);

    disc_f disc(transform(center), transform(radius));
    solid_color_brush_rgb8u brush(convert_color_to<ggo::rgb_8u>(color));
    alpha_blender_rgb8u blender(color.a());

    return std::make_shared<layer_t<disc_f, ggo::rgb_8u, solid_color_brush_rgb8u, alpha_blender_rgb8u>>(disc, brush, blender);
  };

  std::shared_ptr<const layer<ggo::rgb_8u>> rectangle_animation::layer_at_time(float time, view_transform<float> transform) const
  {
    ggo::pos2_f p1 = ggo::linear_interpolation1d(_p1.begin(), _p1.end(), time);
    ggo::pos2_f p2 = ggo::linear_interpolation1d(_p2.begin(), _p2.end(), time);
    ggo::rgba_32f color = ggo::linear_interpolation1d(_color.begin(), _color.end(), time);

    rect_f rect(transform(p1), transform(p2));
    solid_color_brush_rgb8u brush(convert_color_to<ggo::rgb_8u>(color));
    alpha_blender_rgb8u blender(color.a());

    return std::make_shared<layer_t<rect_f, ggo::rgb_8u, solid_color_brush_rgb8u, alpha_blender_rgb8u>>(rect, brush, blender);
  }






  canvas<ggo::rgb_8u> canvas_at_time(const animation& anim, float time, size render_size)
  {
    canvas<ggo::rgb_8u> canvas;

    const auto transform = ggo::view_transform<float>::make_fit(anim._view, render_size);

    for (const auto & layer_animation : anim._layer_animations)
    {
      canvas.add_layer(layer_animation->layer_at_time(time, transform));
    }

    return canvas;
  }
}
