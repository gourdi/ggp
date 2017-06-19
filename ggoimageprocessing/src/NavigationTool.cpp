#include "NavigationTool.h"
#include "ImageWidget.h"

/////////////////////////////////////////////////////////////////////
NavigationTool::NavigationTool(ImageWidget & imageWidget)
:
ToolAbc(imageWidget)
{

}

/////////////////////////////////////////////////////////////////////
void NavigationTool::doMousePressEvent(const ggo::pos2i & mousePos)
{

}

////////////////////////////////////////////////////////////////////
void NavigationTool::doMouseReleaseEvent(const ggo::pos2i & mousePos)
{

}

/////////////////////////////////////////////////////////////////////
void NavigationTool::doMouseMoveEvent(const ggo::pos2i & mousePos)
{

}

/////////////////////////////////////////////////////////////////////
void NavigationTool::doWheelEvent(const ggo::pos2i & mousePos, int angle)
{
  if (angle > 0)
  {
    GetImageWidget().zoom(0.75f, mousePos);
  }

  if (angle < 0)
  {
    GetImageWidget().zoom(1.5f, mousePos);
  }
}
