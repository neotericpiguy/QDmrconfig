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

  void setDebug(bool state);
  void updateTabs();
  void clear();

  ConfFile& getConfFile();

private slots:
  void tabSelected();

private:
  ConfFile _confFile;
  bool _isDebug;

  std::string _filename;
  std::unique_ptr<QTabWidget> _tabWidget;
  std::unique_ptr<QVBoxLayout> _layout;
  std::vector<std::unique_ptr<ConfBlockWidget>> _confBlockWidgets;
};

#endif
