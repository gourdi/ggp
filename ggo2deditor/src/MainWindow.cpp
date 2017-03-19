#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qmenubar.h>
#include "MainWindow.h"
#include "CanvasWidget.h"

/////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
:
_renderWidget(new CanvasWidget(this))
{
  setCentralWidget(_renderWidget);

  createMenus();

  //QWidget *topFiller = new QWidget;
  //topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  //infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
  //  "invoke a context menu</i>"));
  //infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  //infoLabel->setAlignment(Qt::AlignCenter);

  //QWidget *bottomFiller = new QWidget;
  //bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  //QVBoxLayout *layout = new QVBoxLayout;
  //layout->setMargin(5);
  //layout->addWidget(topFiller);
  //layout->addWidget(infoLabel);
  //layout->addWidget(bottomFiller);
  //widget->setLayout(layout);
}

/////////////////////////////////////////////////////////////////////
void MainWindow::createMenus()
{
  _shapeMenu = menuBar()->addMenu(tr("&Shapes"));

  _createDiscAction = new QAction(tr("Create &Disc"), this);
  _createDiscAction->setStatusTip(tr("Create a new disc"));
  _shapeMenu->addAction(_createDiscAction);
  connect(_createDiscAction, &QAction::triggered, _renderWidget, &CanvasWidget::createDisc);

  _createPolygonAction = new QAction(tr("Create &Polygon"), this);
  _createPolygonAction->setStatusTip(tr("Create a new polygon"));
  _shapeMenu->addAction(_createPolygonAction);
  connect(_createPolygonAction, &QAction::triggered, _renderWidget, &CanvasWidget::createPolygon);

  _shapeMenu->addSeparator();

  _selectShapeColorAction = new QAction(tr("Select Color..."), this);
  _shapeMenu->addAction(_selectShapeColorAction);
  connect(_selectShapeColorAction, &QAction::triggered, _renderWidget, &CanvasWidget::setShapeColor);
}
