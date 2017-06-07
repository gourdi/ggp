#include <QtWidgets/qapplication.h>
#include "MainWindow.h"

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  if (argc == 2)
  {
    mainWindow.loadFile(argv[1]);
  }

  return app.exec();
}



