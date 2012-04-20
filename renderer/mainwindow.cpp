#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setupMenu();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::loadScene() {
  GLWidget *glw = (GLWidget*)ui->glwidget;

  QString filename = QFileDialog::getOpenFileName(
    this,
    "",
    "",
    "Scenes (*.txt)");

  if( filename.isEmpty() )
    return;

  glw->loadScene(filename.toStdString().c_str());
}

void MainWindow::setRMPoints() {
  GLWidget *glw = (GLWidget*)ui->glwidget;
  glw->renderMode(GLWidget::Points);
}

void MainWindow::setRMWireframe() {
  GLWidget *glw = (GLWidget*)ui->glwidget;
  glw->renderMode(GLWidget::Wireframe);
}

void MainWindow::setRMSolid() {
  GLWidget *glw = (GLWidget*)ui->glwidget;
  glw->renderMode(GLWidget::Solid);
}

void MainWindow::centerView() {
  GLWidget *glw = (GLWidget*)ui->glwidget;
  glw->centerView();
}

void MainWindow::setupMenu() {
  QAction *action;

  QMenu *fileMenu = menuBar()->addMenu("&File");

  action = fileMenu->addAction("&Load Scene...");
  connect(action, SIGNAL(triggered()), this, SLOT(loadScene()));

  fileMenu->addSeparator();

  fileMenu->addAction("E&xit", this, SLOT(close()));

  QMenu *viewMenu = menuBar()->addMenu("&View");

  action = viewMenu->addAction("&Center View");
  connect(action, SIGNAL(triggered()), this, SLOT(centerView()));

  QMenu *renderMode = viewMenu->addMenu("Render &Mode");

  action = renderMode->addAction("&Points");
  connect(action, SIGNAL(triggered(bool)), this, SLOT(setRMPoints()));

  action = renderMode->addAction("&Wireframe");
  connect(action, SIGNAL(triggered(bool)), this, SLOT(setRMWireframe()));

  action = renderMode->addAction("&Solid");
  connect(action, SIGNAL(triggered(bool)), this, SLOT(setRMSolid()));
}

