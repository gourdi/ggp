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

  void loadFile(const std::string & filename);


private slots:

  void load();
  void zoomIn();
  void zoomOut();
  void zoomFit();

private:

  ImageWidget * _imageWidget;
};
