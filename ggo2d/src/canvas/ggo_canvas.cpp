#include "ggo_canvas.h"
#include <ggo_buffer_fill.h>
#include <ggo_string_helpers.h>
#include <tinyxml2.h>
#include <exception>
#include <sstream>

// Shape interface
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  std::unique_ptr<ggo::paintable_shape2d_abc<float>> canvas::shape_abc::make_unique_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    return std::unique_ptr<ggo::paintable_shape2d_abc<float>>(create_render_shape(view, render_width, render_height, vertical_flip));
  }
}

// Disc.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  disc_float * canvas::disc::create_render_disc(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    const pos2f center = from_view_to_render(_disc.center(), view, render_width, render_height, vertical_flip);
    const float radius = from_view_to_render(_disc.radius(), view._size, view._main_direction, render_width, render_height);

    return new disc_float(center, radius);
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::disc::set_from_render_points(const pos2f & p1, const pos2f & p2, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    _disc = disc_float(
      canvas::from_render_to_view(p1, view, render_width, render_height, vertical_flip),
      canvas::from_render_to_view(p2, view, render_width, render_height, vertical_flip));
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::disc::set_from_render_disc(const disc_float & disc, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    _disc.center() = canvas::from_render_to_view(disc.center(), view, render_width, render_height, vertical_flip);
    _disc.radius() = canvas::from_render_to_view(disc.radius(), view._size, view._main_direction, render_width, render_height);
  }

  ///////////////////////////////////////////////////////////////////
  tinyxml2::XMLElement * canvas::disc::create_xml_element(tinyxml2::XMLDocument * document) const
  {
    tinyxml2::XMLElement * disc_element = document->NewElement("disc");
    tinyxml2::XMLElement * radius_element = document->NewElement("radius");
    tinyxml2::XMLElement * center_element = document->NewElement("center");

    const std::string radius_txt = std::to_string(_disc.radius());
    const std::string center_txt = std::to_string(_disc.center().x()) + ';' + std::to_string(_disc.center().y());

    radius_element->InsertEndChild(document->NewText(radius_txt.c_str()));
    center_element->InsertEndChild(document->NewText(center_txt.c_str()));

    disc_element->InsertEndChild(radius_element);
    disc_element->InsertEndChild(center_element);

    return disc_element;
  }
}

// Polygon.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  polygon2d_float * canvas::polygon::create_render_polygon(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    const int points_count = _polygon.get_points_count();

    std::vector<pos2f> mapped_points(points_count);
    for (int i = 0; i < points_count; ++i)
    {
      mapped_points[i] = from_view_to_render(_polygon.get_point(i), view, render_width, render_height, vertical_flip);
    }

    return new polygon2d_float(std::move(mapped_points));
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::polygon::update_render_point(int index, const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    GGO_ASSERT_BTW(index, 0, _polygon.get_points_count() - 1);

    _polygon.get_point(index) = canvas::from_render_to_view(p, view, render_width, render_height, vertical_flip);
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::polygon::set_from_render_polygon(const ggo::polygon2d_float & polygon, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const int points_count = polygon.get_points_count();

    std::vector<pos2f> mapped_points(points_count);
    for (int i = 0; i < points_count; ++i)
    {
      mapped_points[i] = from_render_to_view(polygon.get_point(i), view, render_width, render_height, vertical_flip);
    }

    _polygon.set_points(std::move(mapped_points));
  }

  ///////////////////////////////////////////////////////////////////
  tinyxml2::XMLElement * canvas::polygon::create_xml_element(tinyxml2::XMLDocument * document) const
  {
    tinyxml2::XMLElement * polygon_element = document->NewElement("polygon");
    tinyxml2::XMLElement * points_element = document->NewElement("points");

    auto point2str = [](const ggo::pos2f & p) { return std::to_string(p.x()) + ';' + std::to_string(p.y()); };
    auto points_txt = ggo::join(_polygon.cbegin(), _polygon.cend(), ";", point2str);

    points_element->InsertEndChild(document->NewText(points_txt.c_str()));
    polygon_element->InsertEndChild(points_element);

    return polygon_element;
  }
}

// Canvas.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  canvas::disc * canvas::create_disc()
  {
    canvas::disc * disc = new canvas::disc();

    _shapes.emplace_back(std::unique_ptr<shape_abc>(disc));

    return disc;
  }

  ///////////////////////////////////////////////////////////////////
  canvas::polygon * canvas::create_polygon()
  {
    canvas::polygon * polygon = new canvas::polygon();

    _shapes.emplace_back(std::unique_ptr<shape_abc>(polygon));

    return polygon;
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::remove_shape(const paintable_shape2d_abc<float> * shape)
  {
    remove_first_if(_shapes, [&](std::unique_ptr<shape_abc> & canvas_shape) { return canvas_shape->get_shape() == shape; });
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::render(void * buffer, const view & view, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
  {
    const pixel_rect clipping = pixel_rect::from_width_height(width, height);

    using shape_t = dyn_paint_shape<float, color_8u, color_8u>;

    std::vector<shape_t> mapped_shapes;

    // Map from canvas to render.
    for (const auto & shape : _shapes)
    {
      shape_t mapped_shape;
      mapped_shape._shape = std::shared_ptr<paintable_shape2d_abc<float>>(shape->create_render_shape(view, width, height, false));
      mapped_shape._brush = std::make_shared<solid_dyn_brush<color_8u>>(shape->_color);
      mapped_shape._blender = std::make_shared<overwrite_dyn_blender<color_8u, color_8u>>();

      mapped_shapes.push_back(mapped_shape);
    }

    // Paint mapped shapes.
    switch (pbf)
    {
    case bgra_8u_yd:
      fill_solid<bgra_8u_yd>(buffer, width, height, line_byte_step, _background_color);
      paint_shapes<bgra_8u_yd, sampling_4x4>(buffer, width, height, line_byte_step, mapped_shapes.begin(), mapped_shapes.end(), clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  float canvas::from_view_to_render(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
  {
    switch (main_dir)
    {
    case main_direction::vertical:
      return render_height * dist / (2.0f * view_size);
    case main_direction::horizontal:
      return render_width * dist / (2.0f * view_size);
    default:
      GGO_FAIL();
      return 0.f;
    }
  }

  /////////////////////////////////////////////////////////////////////
  float canvas::from_render_to_view(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
  {
    switch (main_dir)
    {
    case main_direction::vertical:
      return 2.0f * view_size * dist / render_height;
    case main_direction::horizontal:
      return 2.0f * view_size * dist / render_width;
    default:
      GGO_FAIL();
      return 0.f;
    }
  }

  /////////////////////////////////////////////////////////////////////
  pos2f canvas::from_view_to_render(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    switch (view._main_direction)
    {
    case main_direction::vertical:
    {
      float ratio = float(render_width) / float(render_height);
      float x = ggo::map(p.x() - view._center.x(), -ratio * view._size, ratio * view._size, -0.5f, float(render_width) - 0.5f);
      float y = ggo::map(p.y() - view._center.y(), -view._size, view._size, -0.5f, float(render_height) - 0.5f);
      if (vertical_flip)
      {
        y = render_height - y - 1;
      }
      return pos2f(x, y);
    }
    case main_direction::horizontal:
    {
      float ratio = float(render_height) / float(render_width);
      float x = ggo::map(p.x() - view._center.x(), -view._size, view._size, -0.5f, float(render_width) - 0.5f);
      float y = ggo::map(p.y() - view._center.y(), -ratio * view._size, ratio * view._size, -0.5f, float(render_height) - 0.5f);
      if (vertical_flip)
      {
        y = render_height - y - 1;
      }
      return pos2f(x, y);
    }
    default:
      GGO_FAIL();
      return pos2f(0, 0);
    }
  }

  /////////////////////////////////////////////////////////////////////
  pos2f canvas::from_render_to_view(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    float x = p.x();
    float y = vertical_flip ? render_height - p.y() - 1 : p.y();

    switch (view._main_direction)
    {
    case main_direction::vertical:
    {
      float ratio = float(render_width) / float(render_height);
      x = view._center.x() + ggo::map(x, -0.5f, float(render_width) - 0.5f, -ratio * view._size, ratio * view._size);
      y = view._center.y() + ggo::map(y, -0.5f, float(render_height) - 0.5f, -view._size, view._size);
      break;
    }
    case main_direction::horizontal:
    {
      float ratio = float(render_height) / float(render_width);
      x = view._center.x() + ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -view._size, view._size);
      y = view._center.y() + ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -ratio * view._size, ratio * view._size);
      break;
    }
    default:
      GGO_FAIL();
      break;
    }
    return pos2f(x, y);
  }

  /////////////////////////////////////////////////////////////////////
  pos2i canvas::from_view_to_render_pixel(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const pos2f pf = from_view_to_render(p, view, render_width, render_height, vertical_flip);

    return pos2i(ggo::to<int>(pf.x()), ggo::to<int>(pf.y()));
  }

  /////////////////////////////////////////////////////////////////////
  pos2f canvas::from_render_pixel_to_view(const pos2i & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const pos2f pf(ggo::to<float>(p.x()), ggo::to<float>(p.y()));

    return from_render_to_view(pf, view, render_width, render_height, vertical_flip);
  }
}

// Save.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  std::unique_ptr<tinyxml2::XMLDocument> canvas::create_xml_document() const
  {
    auto document = std::make_unique<tinyxml2::XMLDocument>();

    auto canvas_element = document->NewElement("canvas");
    document->InsertEndChild(canvas_element);

    std::ostringstream oss;
    oss << '#' << ggo::to_hex(_background_color);
    canvas_element->SetAttribute("background_color", oss.str().c_str());

    auto shapes_element = document->NewElement("shapes");
    canvas_element->InsertEndChild(shapes_element);

    for (const auto & shape : _shapes)
    {
      tinyxml2::XMLElement * shape_element = shape->create_xml_element(document.get());

      std::ostringstream oss;
      oss << '#' << ggo::to_hex(shape->_color);
      shape_element->SetAttribute("color", oss.str().c_str());

      shapes_element->InsertEndChild(shape_element);
    }


    return document;
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::save(const char * filename) const
  {
    auto document = create_xml_document();

    tinyxml2::XMLError error = document->SaveFile(filename);
    if (error != tinyxml2::XML_SUCCESS)
    {
      std::ostringstream error_msg;
      error_msg << "xml error: " << error;
      throw std::runtime_error(error_msg.str());
    }
  }

  /////////////////////////////////////////////////////////////////////
  std::string canvas::to_string() const
  {
    auto document = create_xml_document();

    tinyxml2::XMLPrinter printer;
    document->Print(&printer);

    return printer.CStr();
  }
}

// Load.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  canvas::canvas(const std::string & xml, load_delegate * delegate)
  {
    from_string(xml, delegate);
  }

  /////////////////////////////////////////////////////////////////////
  canvas::canvas(const tinyxml2::XMLDocument & document, load_delegate * delegate)
  {
    from_xml_document(document, delegate);
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::from_string(const std::string & xml, load_delegate * delegate)
  {
    tinyxml2::XMLDocument document;

    tinyxml2::XMLError error = document.Parse(xml.c_str());
    if (error != tinyxml2::XML_SUCCESS)
    {
      std::ostringstream error_msg;
      error_msg << "xml error: " << error;
      throw std::runtime_error(error_msg.str());
    }

    from_xml_document(document, delegate);
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::from_xml_document(const tinyxml2::XMLDocument & document, load_delegate * delegate)
  {
    _shapes.clear();

    const tinyxml2::XMLNode * canvas_node = document.FirstChild();
    if (canvas_node == nullptr)
    {
      throw std::runtime_error("missing 'canvas' node");
    }

    const std::string canvas_value = canvas_node->Value();
    if (canvas_value != "canvas")
    {
      throw std::runtime_error(std::string("invalid root node: ") + canvas_value);
    }

    const tinyxml2::XMLElement * shapes_element = canvas_node->FirstChildElement("shapes");
    if (shapes_element == nullptr)
    {
      throw std::runtime_error("missing 'shapes' element");
    }

    const tinyxml2::XMLElement * shape_element = shapes_element->FirstChildElement();
    while (shape_element != nullptr)
    {
      const std::string shape_value = shape_element->Value();
      shape_abc * new_shape = nullptr;

      // Geometry.
      if (shape_value == "disc")
      {
        new_shape = create_disc(*shape_element, delegate);
      }
      else if(shape_value == "polygon")
      {
        new_shape = create_polygon(*shape_element, delegate);
      }
      
      if (new_shape == nullptr)
      {
        throw std::runtime_error(std::string("invalid shape value") + shape_value);
      }

      // Color.
      const std::string color_text = shape_element->Attribute("color");
      if (color_text.empty() == true || color_text[0] != '#')
      {
        throw std::runtime_error(std::string("empty or invalid color: '") + color_text + "'");
      }
      new_shape->_color = ggo::from_hex(color_text);

      shape_element = shape_element->NextSiblingElement();
    }
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::load(const char * filename, load_delegate * delegate)
  {
    tinyxml2::XMLDocument document;
    
    tinyxml2::XMLError error = document.LoadFile(filename);
    if (error != tinyxml2::XML_SUCCESS)
    {
      std::ostringstream error_msg;
      error_msg << "xml error: " << error;
      throw std::runtime_error(error_msg.str());
    }

    from_xml_document(document, delegate);
  }

  /////////////////////////////////////////////////////////////////////
  canvas::shape_abc * canvas::create_disc(const tinyxml2::XMLElement & shape_element, load_delegate * delegate)
  {
    disc * new_disc = create_disc();

    // Radius.
    {
      const tinyxml2::XMLElement * radius_element = shape_element.FirstChildElement("radius");
      if (radius_element == nullptr)
      {
        throw std::runtime_error("missing radius");
      }

      float radius = 0.f;
      tinyxml2::XMLError error = radius_element->QueryFloatText(&radius);
      if (error != tinyxml2::XML_SUCCESS || radius < 0.f)
      {
        throw std::runtime_error("invalid radius");
      }

      new_disc->get_disc().set_radius(radius);
    }
    
    // Center.
    {
      const tinyxml2::XMLElement * center_element = shape_element.FirstChildElement("center");
      if (center_element == nullptr)
      {
        throw std::runtime_error("missing center");
      }

      const std::string center_text = center_element->GetText();
      auto tokens = ggo::split(center_text, ';');
      if (tokens.size() != 2)
      {
        throw std::runtime_error("invalid center");
      }

      new_disc->get_disc().set_center(ggo::to<float>(tokens[0]), ggo::to<float>(tokens[1]));
    }

    if (delegate != nullptr)
    {
      delegate->on_create_disc(new_disc);
    }

    return new_disc;
  }

  /////////////////////////////////////////////////////////////////////
  canvas::shape_abc * canvas::create_polygon(const tinyxml2::XMLElement & shape_element, load_delegate * delegate)
  {
    polygon * new_polygon = create_polygon();

    const tinyxml2::XMLElement * points_element = shape_element.FirstChildElement("points");
    if (points_element == nullptr)
    {
      throw std::runtime_error("missing points");
    }

    const std::string points_text = points_element->GetText();
    auto tokens = ggo::split(points_text, ';');
    if (ggo::is_odd(tokens.size()) == true)
    {
      throw std::runtime_error("invalid points");
    }

    auto it = tokens.cbegin();
    while (it != tokens.cend())
    {
      const float x = ggo::to<float>(*it++);
      const float y = ggo::to<float>(*it++);
      new_polygon->get_polygon().add_point(x, y);
    }

    if (delegate != nullptr)
    {
      delegate->on_create_polygon(new_polygon);
    }

    return new_polygon;
  }
}

