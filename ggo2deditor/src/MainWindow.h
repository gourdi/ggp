#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmenu.h>

class RenderWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();

private:

  void createMenus();

private:

  RenderWidget * _renderWidget;
  QMenu * _shapeMenu;
  QAction * _createDiscAction;
  QAction * _createPolygonAction;
};
