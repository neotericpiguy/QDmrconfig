#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QtWidgets>
#include <memory>

#include "ListWidget.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  virtual ~MainWindow();
  void loadFile(const QString& fileName);
  void setCurrentFile(const QString& fileName);
  void closeEvent(QCloseEvent* event) override;

private:
  std::unique_ptr<QPlainTextEdit> textEdit;
  std::unique_ptr<ListWidget> listWidget;
};

#endif
