#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qfiledialog.h>
#include "MainWindow.h"
#include "CanvasWidget.h"

/////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
:
_canvasWidget(new CanvasWidget(this))
{
  setCentralWidget(_canvasWidget);

  createMenus();

  _canvasWidget->setFocus();
}

/////////////////////////////////////////////////////////////////////
void MainWindow::createMenus()
{
  {
    _fileMenu = menuBar()->addMenu(tr("&File"));

    _openFileAction = new QAction(tr("Open..."));
    _openFileAction->setStatusTip(tr("Open a file"));
    _openFileAction->setShortcut(QKeySequence::Open);
    _fileMenu->addAction(_openFileAction);
    connect(_openFileAction, &QAction::triggered, this, &MainWindow::loadFile);

    _saveFileAction = new QAction(tr("Save"));
    _saveFileAction->setStatusTip(tr("Save current file"));
    _saveFileAction->setShortcut(QKeySequence::Save);
    _fileMenu->addAction(_saveFileAction);
    connect(_saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
  }

  {
    _shapeMenu = menuBar()->addMenu(tr("&Shapes"));

    _createDiscAction = new QAction(tr("Create &Disc"), this);
    _createDiscAction->setStatusTip(tr("Create a new disc"));
    _shapeMenu->addAction(_createDiscAction);
    connect(_createDiscAction, &QAction::triggered, _canvasWidget, &CanvasWidget::createDisc);

    _createPolygonAction = new QAction(tr("Create &Polygon"), this);
    _createPolygonAction->setStatusTip(tr("Create a new polygon"));
    _shapeMenu->addAction(_createPolygonAction);
    connect(_createPolygonAction, &QAction::triggered, _canvasWidget, &CanvasWidget::createPolygon);

    _shapeMenu->addSeparator();

    _selectShapeColorAction = new QAction(tr("Select Color..."), this);
    _shapeMenu->addAction(_selectShapeColorAction);
    connect(_selectShapeColorAction, &QAction::triggered, _canvasWidget, &CanvasWidget::setShapeColor);
  }
}

/////////////////////////////////////////////////////////////////////
void MainWindow::loadFile()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Load Canvas"), "",
    tr("Canvas file (*.Canvas)"));

  if (fileName.isEmpty() == false)
  {
    try
    {
      _canvasWidget->loadCanvas(fileName);
    }
    catch (...)
    {

    }
  }
}

/////////////////////////////////////////////////////////////////////
void MainWindow::saveFile()
{
  QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save Canvas"), "",
    tr("Canvas file (*.Canvas)"));

  if (fileName.isEmpty() == false)
  {
    try
    {
      _canvasWidget->saveCanvas(fileName);
    }
    catch (...)
    {

    }
  }
}

