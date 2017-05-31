#ifndef __GGO_IMAGE_WIDGET__
#define __GGO_IMAGE_WIDGET__

#include <QWidget>
#include <memory>
#include <ggo_image_processing.h>

class ImageWidget : public QWidget
{
  Q_OBJECT

public:

  ImageWidget(QWidget * parent);

  void loadImage(const QString & filename);
  void zoomIn();
  void zoomOut();
  void zoomFit();

private:

  void zoom(float zoomFactor);
  void zoom(float zoomFactor, const ggo::pos2f & center);

  ggo::image_view get_view() const;

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *eventPress) override;
  void mouseReleaseEvent(QMouseEvent *releaseEvent) override;
  void mouseMoveEvent(QMouseEvent *eventMove) override;
  void wheelEvent(QWheelEvent *event) override;

private:

  QImage _image;
  std::unique_ptr<ggo::image_processor> _image_processor;
  ggo::basis2d<float> _view_basis;

  bool _is_mouse_down = false;
  ggo::pos2i _mouse_down_pos;
  ggo::basis2d<float> _mouse_down_view_basis;
};

#endif
