#ifndef __GGO_IMAGE_WIDGET__
#define __GGO_IMAGE_WIDGET__

#include <QWidget>
#include <memory>
#include <ggo_image_processing.h>

class ToolAbc;

class ImageWidget : public QWidget
{
  Q_OBJECT

public:

  ImageWidget(QWidget * parent);

  void loadImage(const std::string & filename);
  void zoomIn();
  void zoomOut();
  void zoomFit();

private:

  void zoom(float zoomFactor);
  void zoom(float zoomFactor, const ggo::pos2i & fixed_view_point);

  ggo::image_view get_view() const;

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:

  QImage _image;
  std::unique_ptr<ggo::image_processor> _image_processor;
  ggo::basis2d<float> _view_basis;
  std::unique_ptr<ToolAbc> _tool;

  bool _is_mouse_down = false;
  ggo::pos2i _mouse_down_pos;
  ggo::basis2d<float> _mouse_down_view_basis;
};

#endif
