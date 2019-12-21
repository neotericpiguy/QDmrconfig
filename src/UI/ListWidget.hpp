#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <fstream>
#include <memory>

#include "ConfBlock.hpp"
#include "ConfBlockWidget.hpp"

class ListWidget : public QWidget
{
  Q_OBJECT

public:
  ListWidget();
  virtual ~ListWidget();

  void loadFile(const std::string& filename);
  void saveFile();
  void updateTabs();
  bool isModified() const;

private:
  std::string _filename;
  std::unique_ptr<QTabWidget> tabWidget;
  std::map<int, ConfBlock> confBlocks;
};

#endif
