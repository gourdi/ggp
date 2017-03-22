#ifndef __GGO_CANVAS_WIDGET__
#define __GGO_CANVAS_WIDGET__

#include <QtWidgets/qwidget.h>
#include "ShapeFactory.h"

class CanvasWidget : public QWidget
{
  Q_OBJECT

public:

  CanvasWidget(QWidget * parent);

protected:

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *eventPress) override;
  void mouseReleaseEvent(QMouseEvent *releaseEvent) override;
  void mouseMoveEvent(QMouseEvent *eventMove) override;
  void wheelEvent(QWheelEvent *event) override;

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  ggo::canvas::view getCanvasView() const;

  ShapeHandler * hitTest(int x, int y, int width, int height, const ggo::canvas::view & view);

public slots:

  void createDisc();
  void createPolygon();
  void setShapeColor();

private:

  QImage _image;
  ggo::canvas _canvas;
  std::unique_ptr<ShapeFactory> _shapeFactory;
  int _zoomIndex;

  std::vector<std::unique_ptr<ShapeHandler>> _shapeHandlers;
  ShapeHandler* _edition = nullptr;
  ShapeHandler* _mouseDownShape = nullptr;
  bool _hasMouveMoved = false;
  std::vector<ShapeHandler*> _selection;
  ggo::color_8u _color;
};

#endif
