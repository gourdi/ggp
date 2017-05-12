#include "MainWindow.h"
#include <ggo_load_image.h>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QScrollBar>
#include <QMenuBar>

/////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
:
_imageWidget(new ImageWidget(this))
{
  setCentralWidget(_imageWidget);

  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

  QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
  QAction * loadFileAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::loadFile);
  loadFileAction->setShortcut(QKeySequence::Open);

  QMenu * zoomMenu = menuBar()->addMenu(tr("&Zoom"));
  QAction * zoomInAction = zoomMenu->addAction(tr("Zoom In"), this, &MainWindow::zoomIn);
  QAction * zoomOutAction = zoomMenu->addAction(tr("Zoom Out"), this, &MainWindow::zoomOut);
  QAction * zoomFitAction = zoomMenu->addAction(tr("zoom Fit"), this, &MainWindow::zoomFit);
}

/////////////////////////////////////////////////////////////////////
void MainWindow::loadFile()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Load Image"), "",
    tr("Canvas file (*.*)"));

  if (fileName.isEmpty() == false)
  {
    _imageWidget->loadImage(fileName);
  }
}

/////////////////////////////////////////////////////////////////////
void MainWindow::zoomIn()
{
  _imageWidget->zoomIn();
}

/////////////////////////////////////////////////////////////////////
void MainWindow::zoomOut()
{
  _imageWidget->zoomOut();
}

/////////////////////////////////////////////////////////////////////
void MainWindow::zoomFit()
{
  _imageWidget->zoomFit();
}
