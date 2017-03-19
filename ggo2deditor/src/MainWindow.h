#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmenu.h>

class CanvasWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();

private:

  void createMenus();

private:

  CanvasWidget * _renderWidget;
  QMenu * _shapeMenu;
  QAction * _createDiscAction;
  QAction * _createPolygonAction;
  QAction * _selectShapeColorAction;
};
