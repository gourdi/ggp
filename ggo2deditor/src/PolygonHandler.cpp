#include "PolygonHandler.h"

/////////////////////////////////////////////////////////////////////
void PolygonHandler::Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const
{
  std::unique_ptr<ggo::polygon2d_float> polygon(_polygon->create_render_polygon(view, width, height, true));

  int points_count = polygon->get_points_count();
  for (int i = 0, j = points_count - 1; i < points_count; j = i++)
  {
    const ggo::pos2f & p1 = polygon->get_point(i);
    const ggo::pos2f & p2 = polygon->get_point(j);

    painter.setPen(QPen(Qt::black));
    painter.drawLine(ggo::to<int>(p1.x()), ggo::to<int>(p1.y()), ggo::to<int>(p2.x()), ggo::to<int>(p2.y()));

    painter.setPen(QPen(Qt::white, 1, Qt::DotLine));
    painter.drawLine(ggo::to<int>(p1.x()), ggo::to<int>(p1.y()), ggo::to<int>(p2.x()), ggo::to<int>(p2.y()));
  }

  for (const auto & point : *polygon)
  {
    PaintHandle(painter, point);
  }
}

/////////////////////////////////////////////////////////////////////
int PolygonHandler::HitTest(int x, int y, int width, int height, const ggo::canvas::view & view) const
{
  std::unique_ptr<ggo::polygon2d_float> polygon(_polygon->create_render_polygon(view, width, height, true));

  const int points_count = polygon->get_points_count();
  for (int i = 0; i < points_count; ++i)
  {
    const ggo::pos2f & p = polygon->get_point(i);

    if (HitTestHandle(ggo::to<int>(p.x()), ggo::to<int>(p.y()), x, y) == true)
    {
      return i;
    }
  }

  return -1;
}

/////////////////////////////////////////////////////////////////////
bool PolygonHandler::OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  _mouse_down_index = HitTest(x, y, width, height, view);
  if (_mouse_down_index >= 0)
  {
    return true; // Consume event.
  }

  return false;
}

/////////////////////////////////////////////////////////////////////
bool PolygonHandler::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  _mouse_down_index = -1;
  return false;
}

/////////////////////////////////////////////////////////////////////
ShapeHandler::MouseMoveData PolygonHandler::OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  MouseMoveData mouseMoveData;

  if (_mouse_down_index >= 0)
  {
    ggo::pos2f render_point(static_cast<float>(x), static_cast<float>(y));
    _polygon->update_render_point(_mouse_down_index, render_point, view, width, height, true);

    mouseMoveData._update_widget = true;
  }

  int pointIndex = _mouse_down_index >= 0 ? _mouse_down_index : HitTest(x, y, width, height, view);

  mouseMoveData._cursor = pointIndex >= 0 ? Qt::SizeAllCursor : Qt::ArrowCursor;

  return mouseMoveData;
}

/////////////////////////////////////////////////////////////////////
void PolygonHandler::SetAnchor(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  std::unique_ptr<ggo::polygon2d_float> render_polygon(_polygon->create_render_polygon(view, width, height, true));

  _polygon_anchor = std::make_unique<PolygonAnchor>(*render_polygon, x, y);
}

/////////////////////////////////////////////////////////////////////
void PolygonHandler::SetPosition(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  GGO_ASSERT(_polygon_anchor);
  if (_polygon_anchor)
  {
    ggo::polygon2d_float polygon(_polygon_anchor->_polygon);
    polygon.move(static_cast<float>(x - _polygon_anchor->_x), static_cast<float>(y - _polygon_anchor->_y));
    _polygon->set_from_render_polygon(polygon, view, width, height, true);
  }
}

