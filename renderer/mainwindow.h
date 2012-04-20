#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void loadScene();

  void setRMPoints();
  void setRMSolid();
  void setRMWireframe();

  void centerView();

private:
  Ui::MainWindow *ui;

  void setupMenu();
};

#endif // MAINWINDOW_H
