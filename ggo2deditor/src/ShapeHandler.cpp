#include "ShapeHandler.h"

/////////////////////////////////////////////////////////////////////
QRect ShapeHandler::GetHandleRect(int x, int y)
{
  return QRect(x - half_tolerance, y - half_tolerance, tolerance, tolerance);
}

/////////////////////////////////////////////////////////////////////
bool ShapeHandler::HitTestHandle(int handle_x, int handle_y, int test_x, int test_y)
{
  return GetHandleRect(handle_x, handle_y).contains(test_x, test_y);
}

/////////////////////////////////////////////////////////////////////
void ShapeHandler::PaintHandle(QPainter & painter, int x, int y)
{
  QRect rect = GetHandleRect(x, y);

  painter.setPen(QPen(Qt::NoPen));
  painter.setBrush(QBrush(Qt::white));
  painter.drawRect(rect);

  painter.setPen(QPen(Qt::black));
  painter.setBrush(QBrush(Qt::black, Qt::Dense4Pattern));
  painter.drawRect(rect);
}

/////////////////////////////////////////////////////////////////////
void ShapeHandler::PaintHandle(QPainter & painter, const ggo::pos2f & p)
{
  PaintHandle(painter, ggo::to<int>(p.x()), ggo::to<int>(p.y()));
}
