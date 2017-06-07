#include "MainWindow.h"
#include <ggo_load_image.h>
#include <QApplication>
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
  QAction * loadFileAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::load);
  loadFileAction->setShortcut(QKeySequence::Open);

  QMenu * zoomMenu = menuBar()->addMenu(tr("&Zoom"));
  QAction * zoomInAction = zoomMenu->addAction(tr("Zoom In"), this, &MainWindow::zoomIn);
  QAction * zoomOutAction = zoomMenu->addAction(tr("Zoom Out"), this, &MainWindow::zoomOut);
  QAction * zoomFitAction = zoomMenu->addAction(tr("Zoom Fit"), this, &MainWindow::zoomFit);
}

/////////////////////////////////////////////////////////////////////
void MainWindow::loadFile(const std::string & filename)
{
  if (filename.empty() == false)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents(); // Otherwise the cursor does not change.
    _imageWidget->loadImage(filename);
    QApplication::restoreOverrideCursor();
  }
}

/////////////////////////////////////////////////////////////////////
void MainWindow::load()
{
  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Load Image"), "",
    tr("Canvas file (*.*)"));

  if (fileName.isEmpty() == false)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents(); // Otherwise the cursor does not change.
    _imageWidget->loadImage(fileName.toUtf8().data());
    QApplication::restoreOverrideCursor();
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
