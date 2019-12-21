#ifndef CONFFILEWIDGET_H
#define CONFFILEWIDGET_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <fstream>
#include <memory>

#include "ConfFile.hpp"

class ConfFileWidget : public QWidget
{
  Q_OBJECT

public:
  ConfFileWidget();
  virtual ~ConfFileWidget();

  void updateTabs();

  ConfFile& getConfFile();

private:
  ConfFile _confFile;

  std::string _filename;
  std::unique_ptr<QTabWidget> _tabWidget;
};

#endif
