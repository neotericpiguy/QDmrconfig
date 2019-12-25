#ifndef CONFFILEWIDGET_H
#define CONFFILEWIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QtWidgets>
#include <fstream>
#include <memory>

#include "ConfBlockWidget.hpp"
#include "ConfFile.hpp"

class ConfFileWidget : public QWidget
{
  Q_OBJECT

public:
  ConfFileWidget();
  virtual ~ConfFileWidget();

  void updateTabs();

  ConfFile& getConfFile();

private slots:
  void tabSelected();

private:
  ConfFile _confFile;

  std::string _filename;
  std::unique_ptr<QTabWidget> _tabWidget;
  std::unique_ptr<QVBoxLayout> _layout;
  std::vector<std::unique_ptr<ConfBlockWidget>> _confBlockWidgets;
};

#endif
