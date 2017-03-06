#include "PolygonHandler.h"

/////////////////////////////////////////////////////////////////////
void PolygonHandler::Draw(QPainter & painter, int width, int height, const ggo::canvas::view & view) const
{

}

/////////////////////////////////////////////////////////////////////
bool PolygonHandler::OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  return false;
}

/////////////////////////////////////////////////////////////////////
bool PolygonHandler::OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, const ggo::canvas::view & view)
{
  return false;
}

/////////////////////////////////////////////////////////////////////
ShapeHandler::MouseMoveData PolygonHandler::OnMouseMove(int x, int y, int width, int height, const ggo::canvas::view & view)
{
  return MouseMoveData();
}


