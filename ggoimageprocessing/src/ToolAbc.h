#ifndef __GGO_VIVELLE_TOOL_ABC__
#define __GGO_VIVELLE_TOOL_ABC__

#include <QMouseEvent>
#include <ggo_vec.h>

class ImageWidget;

class ToolAbc
{
public:

  ToolAbc(ImageWidget & imageWidget);

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

  ImageWidget & GetImageWidget() { return _imageWidget; }

private:

  virtual void doMousePressEvent(const ggo::pos2i & mousePos) = 0;
  virtual void doMouseReleaseEvent(const ggo::pos2i & mousePos) = 0;
  virtual void doMouseMoveEvent(const ggo::pos2i & mousePos) = 0;
  virtual void doWheelEvent(const ggo::pos2i & mousePos, int angle) = 0;

private:

  ImageWidget & _imageWidget;
};

#endif
