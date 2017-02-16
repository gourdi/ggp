#include <QtWidgets/qwidget.h>

class RenderWidget : public QWidget
{
  Q_OBJECT

public:

  RenderWidget(QWidget * parent);

protected:

  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  void resizeImage(QImage * image, const QSize & newSize);

private:

  QImage image;
};
