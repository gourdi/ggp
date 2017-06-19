#include "ToolAbc.h"
#include "ImageWidget.h"

/////////////////////////////////////////////////////////////////////
ToolAbc::ToolAbc(ImageWidget & imageWidget)
:
_imageWidget(imageWidget)
{

}

/////////////////////////////////////////////////////////////////////
void ToolAbc::mousePressEvent(QMouseEvent *event)
{
  doMousePressEvent({ event->pos().x(), _imageWidget.height() - event->pos().y() - 1 });
}

/////////////////////////////////////////////////////////////////////
void ToolAbc::mouseReleaseEvent(QMouseEvent *event)
{
  doMouseReleaseEvent({ event->pos().x(), _imageWidget.height() - event->pos().y() - 1 });
}

/////////////////////////////////////////////////////////////////////
void ToolAbc::mouseMoveEvent(QMouseEvent *event)
{
  doMouseMoveEvent({ event->pos().x(), _imageWidget.height() - event->pos().y() - 1 });
}

/////////////////////////////////////////////////////////////////////
void ToolAbc::wheelEvent(QWheelEvent *event)
{
  doWheelEvent({ event->pos().x(), _imageWidget.height() - event->pos().y() - 1 }, event->angleDelta().y());
}
