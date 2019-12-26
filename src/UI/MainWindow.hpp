#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QtWidgets>
#include <memory>

#include "ConfFileWidget.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  virtual ~MainWindow();

  void loadFile(const QString& fileName);
  void closeEvent(QCloseEvent* event) override;
  void setDebug(bool state);

private:
  std::unique_ptr<ConfFileWidget> _confFileWidget;
};

#endif
