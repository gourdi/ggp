#ifndef __GGO_CANVAS_WIDGET__
#define __GGO_CANVAS_WIDGET__

#include <QtWidgets/qwidget.h>
#include "ShapeFactory.h"

class CanvasWidget : public QWidget, public ggo::canvas::load_delegate
{
  Q_OBJECT

public:

  CanvasWidget(QWidget * parent);

  void loadCanvas(const QString & filename);
  void saveCanvas(const QString & filename) const;

private:

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *eventPress) override;
  void mouseReleaseEvent(QMouseEvent *releaseEvent) override;
  void mouseMoveEvent(QMouseEvent *eventMove) override;
  void wheelEvent(QWheelEvent *event) override;

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  void on_create_disc(ggo::canvas::disc * disc) override;
  void on_create_polygon(ggo::canvas::polygon * polygon) override;

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
  ShapeHandler* _mouseDownShape = nullptr;
  bool _hasMouveMoved = false;
  std::vector<ShapeHandler*> _selection;
  ggo::color_8u _color = ggo::black_8u();
};

#endif
