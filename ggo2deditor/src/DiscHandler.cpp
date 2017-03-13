#include "DiscHandler.h"

namespace
{
  /////////////////////////////////////////////////////////////////////
  void UpdateDiscFromEdge(ggo::canvas::disc & disc, DiscHandler::Modifier modifier,
    const ggo::pos2f & mouse_down_center, float mouse_down_radius,
    int x, int y, int width, int height, const ggo::canvas::view & view)
  {
    switch (modifier)
    {
    case DiscHandler::Modifier::bottom:
    {
      const ggo::pos2f p1(mouse_down_center.x(), mouse_down_center.y() + mouse_down_radius);
      const ggo::pos2f p2(mouse_down_center.x(), static_cast<float>(y));
      disc.set_from_render_points(p1, p2, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::top:
    {
      const ggo::pos2f p1(mouse_down_center.x(), mouse_down_center.y() - mouse_down_radius);
      const ggo::pos2f p2(mouse_down_center.x(), static_cast<float>(y));
      disc.set_from_render_points(p1, p2, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::left:
    {
      const ggo::pos2f p1(mouse_down_center.x() + mouse_down_radius, mouse_down_center.y());
      const ggo::pos2f p2(static_cast<float>(x), mouse_down_center.y());
      disc.set_from_render_points(p1, p2, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::right:
    {
      const ggo::pos2f p1(mouse_down_center.x() - mouse_down_radius, mouse_down_center.y());
      const ggo::pos2f p2(static_cast<float>(x), mouse_down_center.y());
      disc.set_from_render_points(p1, p2, view, width, height, true);
      break;
    }
    default:
      GGO_FAIL();
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  void UpdateDiscFromCorner(ggo::canvas::disc & disc, DiscHandler::Modifier modifier,
    const ggo::pos2f & mouse_down_center, float mouse_down_radius,
    int x, int y, int width, int height, const ggo::canvas::view & view)
  {
    switch (modifier)
    {
    case DiscHandler::Modifier::bottom_left:
    {
      const ggo::pos2f p(mouse_down_center.x() + mouse_down_radius, mouse_down_center.y() + mouse_down_radius);
      const float radius = 0.5f * std::max(std::abs(p.x() - static_cast<float>(x)), std::abs(p.y() - static_cast<float>(y)));
      const float dx = x < p.x() ? -radius : radius;
      const float dy = y < p.y() ? -radius : radius;
      disc.set_from_render_disc({ { p.x() + dx, p.y() + dy }, radius }, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::bottom_right:
    {
      const ggo::pos2f p(mouse_down_center.x() - mouse_down_radius, mouse_down_center.y() + mouse_down_radius);
      const float radius = 0.5f * std::max(std::abs(p.x() - static_cast<float>(x)), std::abs(p.y() - static_cast<float>(y)));
      const float dx = x < p.x() ? -radius : radius;
      const float dy = y < p.y() ? -radius : radius;
      disc.set_from_render_disc({ { p.x() + dx, p.y() + dy }, radius }, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::top_left:
    {
      const ggo::pos2f p(mouse_down_center.x() + mouse_down_radius, mouse_down_center.y() - mouse_down_radius);
      const float radius = 0.5f * std::max(std::abs(p.x() - static_cast<float>(x)), std::abs(p.y() - static_cast<float>(y)));
      const float dx = x < p.x() ? -radius : radius;
      const float dy = y < p.y() ? -radius : radius;
      disc.set_from_render_disc({ { p.x() + dx, p.y() + dy }, radius }, view, width, height, true);
      break;
    }
    case DiscHandler::Modifier::top_right:
    {
      const ggo::pos2f p(mouse_down_center.x() - mouse_down_radius, mouse_down_center.y() - mouse_down_radius);
      const float radius = 0.5f * std::max(std::abs(p.x() - static_cast<float>(x)), std::abs(p.y() - static_cast<float>(y)));
      const float dx = x < p.x() ? -radius : radius;
      const float dy = y < p.y() ? -radius : radius;
      disc.set_from_render_disc({ { p.x() + dx, p.y() + dy }, radius }, view, width, height, true);
      break;
    }
    default:
      GGO_FAIL();
      break;
    }
  }
}

/////////////////////////////////////////////////////////////////////
void DiscHandler::Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const
{
  std::unique_ptr<ggo::disc_float> render_disc(_disc->create_render_disc(view, width, height, true));

  ggo::pixel_rect rect(render_disc->get_bounding_rect());

  painter.setPen(QPen(Qt::black));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect.left(), rect.bottom(), rect.width(), rect.height());

  painter.setPen(QPen(Qt::white, 1, Qt::DotLine));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect.left(), rect.bottom(), rect.width(), rect.height());

  PaintHandle(painter, rect.left(), rect.bottom());
  PaintHandle(painter, rect.left(), rect.top());
  PaintHandle(painter, rect.right(), rect.bottom());
  PaintHandle(painter, rect.right(), rect.top());
}

/////////////////////////////////////////////////////////////////////
bool DiscHandler::OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  if (button == Qt::LeftButton)
  {
    _modifier = HitTest(x, y, width, height, view);
    if (_modifier != Modifier::none)
    {
      _mouse_down_center = ggo::canvas::from_view_to_render(_disc->get_disc().center(), view, width, height, true);
      _mouse_down_radius = ggo::canvas::from_view_to_render(_disc->get_disc().radius(), view._size, view._main_direction, width, height);

      return true; // Consume event.
    }
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
bool DiscHandler::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  _modifier = Modifier::none;
  return false;
}

/////////////////////////////////////////////////////////////////////
ShapeHandler::MouseMoveData DiscHandler::OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  MouseMoveData mouseMoveData;

  // Update disc.
  mouseMoveData._update_widget = true;
  switch (_modifier)
  {
  case Modifier::none:
    mouseMoveData._update_widget = false;
    break;
  case Modifier::bottom:
  case Modifier::top:
  case Modifier::left:
  case Modifier::right:
    UpdateDiscFromEdge(*_disc, _modifier, _mouse_down_center, _mouse_down_radius, x, y, width, height, view);
    break;
  case Modifier::bottom_left:
  case Modifier::bottom_right:
  case Modifier::top_left:
  case Modifier::top_right:
    UpdateDiscFromCorner(*_disc, _modifier, _mouse_down_center, _mouse_down_radius, x, y, width, height, view);
    break;
  }

  // Select the appropriate cursor.
  Modifier cursor_modifier = _modifier == Modifier::none ? HitTest(x, y, width, height, view) : _modifier;
  switch (cursor_modifier)
  {
  case Modifier::none:
    mouseMoveData._cursor = Qt::ArrowCursor;
    break;
  case Modifier::bottom:
    mouseMoveData._cursor = Qt::SizeVerCursor;
    break;
  case Modifier::top:
    mouseMoveData._cursor = Qt::SizeVerCursor;
    break;
  case Modifier::left:
     mouseMoveData._cursor = Qt::SizeHorCursor;
    break;
  case Modifier::right:
     mouseMoveData._cursor = Qt::SizeHorCursor;
    break;
  case Modifier::bottom_left:
     mouseMoveData._cursor = Qt::SizeFDiagCursor;
    break;
  case Modifier::bottom_right:
     mouseMoveData._cursor = Qt::SizeBDiagCursor;
    break;
  case Modifier::top_left:
     mouseMoveData._cursor = Qt::SizeBDiagCursor;
    break;
  case Modifier::top_right:
     mouseMoveData._cursor = Qt::SizeFDiagCursor;
    break;
  }

  return mouseMoveData;
}

/////////////////////////////////////////////////////////////////////
void DiscHandler::SetAnchor(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  std::unique_ptr<ggo::disc_float> render_disc(_disc->create_render_disc(view, width, height, true));

  _disc_anchor = std::make_unique<DiscAnchor>(*render_disc, x, y);
}

/////////////////////////////////////////////////////////////////////
void DiscHandler::SetPosition(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  GGO_ASSERT(_disc_anchor);
  if (_disc_anchor)
  {
    ggo::disc_float disc(_disc_anchor->_disc);
    disc.center().x() += x - _disc_anchor->_x;
    disc.center().y() += y - _disc_anchor->_y;
    _disc->set_from_render_disc(disc, view, width, height, true);
  }
}

/////////////////////////////////////////////////////////////////////
DiscHandler::Modifier DiscHandler::HitTest(int x, int y, int width, int height, const ggo::canvas::view & view) const
{
  std::unique_ptr<ggo::disc_float> render_disc(_disc->create_render_disc(view, width, height, true));

  ggo::pixel_rect rect(render_disc->get_bounding_rect());

  // Corners.
  if (HitTestHandle(rect.left(), rect.bottom(), x, y) == true)
  {
    return Modifier::bottom_left;
  }
  if (HitTestHandle(rect.left(), rect.top(), x, y) == true)
  {
    return Modifier::top_left;
  }
  if (HitTestHandle(rect.right(), rect.bottom(), x, y) == true)
  {
    return Modifier::bottom_right;
  }
  if (HitTestHandle(rect.right(), rect.top(), x, y) == true)
  {
    return Modifier::top_right;
  }

  // Edges.
  if (QRect(rect.left() - half_tolerance, rect.bottom(), tolerance, rect.height()).contains(x, y) == true)
  {
    return Modifier::left;
  }
  if (QRect(rect.right() - half_tolerance, rect.bottom(), tolerance, rect.height()).contains(x, y) == true)
  {
    return Modifier::right;
  }
  if (QRect(rect.left(), rect.bottom() - half_tolerance, rect.width(), tolerance).contains(x, y) == true)
  {
    return Modifier::bottom;
  }
  if (QRect(rect.left(), rect.top() - half_tolerance, rect.width(), tolerance).contains(x, y) == true)
  {
    return Modifier::top;
  }

  return Modifier::none;
}
