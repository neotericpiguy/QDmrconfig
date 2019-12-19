#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <fstream>
#include <memory>

#include "ConfBlock.hpp"

class ListWidget : public QWidget
{
  Q_OBJECT

public:
  ListWidget();
  virtual ~ListWidget();

  void loadFile(const std::string& filename);
  void updateTabs();

private:
  std::unique_ptr<QTabWidget> tabWidget;

  std::map<int, ConfBlock> confBlocks;
};

#endif
