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

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:

  QImage _image;
  std::unique_ptr<ggo::image_processor> _image_processor;
  ggo::basis2d<float> _view_basis;
};

#endif
