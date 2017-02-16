#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgridlayout.h>
#include "MainWindow.h"
#include "RenderWidget.h"

MainWindow::MainWindow()
{
  QPushButton * button1 = new QPushButton("Hello world !");
  QPushButton * button2 = new QPushButton("Toto l'asticot !");

  RenderWidget * renderWidget = new RenderWidget(this);

  QGridLayout * mainLayout = new QGridLayout;
  mainLayout->addWidget(renderWidget, 0, 0, 1, 2);
  mainLayout->addWidget(button1, 1, 0);
  mainLayout->addWidget(button2, 1, 1);
  mainLayout->setRowStretch(0, 1);
  setLayout(mainLayout);
}
