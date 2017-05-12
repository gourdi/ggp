#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include "ImageWidget.h"

class CanvasWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();

private slots:

  void loadFile();
  void zoomIn();
  void zoomOut();
  void zoomFit();

private:

  ImageWidget * _imageWidget;
};
