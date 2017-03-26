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

private slots:

  void loadFile();
  void saveFile();

private:

  CanvasWidget * _canvasWidget;
  QMenu * _fileMenu;
  QAction * _openFileAction;
  QAction * _saveFileAction;
  QMenu * _shapeMenu;
  QAction * _createDiscAction;
  QAction * _createPolygonAction;
  QAction * _selectShapeColorAction;
};
