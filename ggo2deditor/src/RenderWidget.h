#ifndef __GGO_RENDER_WIDGET__
#define __GGO_RENDER_WIDGET__

#include <QtWidgets/qwidget.h>
#include <ggo_canvas.h>

class ShapeFactory
{
public:

  virtual void OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas) = 0;
  virtual void OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas) = 0;
  virtual bool OnMouseMove(int x, int y, int width, int height, ggo::canvas & canvas) = 0;
};

class RenderWidget : public QWidget
{
  Q_OBJECT

public:

  RenderWidget(QWidget * parent);

protected:

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *eventPress) override;
  void mouseReleaseEvent(QMouseEvent *releaseEvent) override;
  void mouseMoveEvent(QMouseEvent *eventMove) override;

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

private:

  QImage _image;
  ggo::canvas _canvas;
  std::unique_ptr<ShapeFactory> _shapeFactory;
};

#endif
