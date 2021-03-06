#ifndef CONFFILEWIDGET_HPP
#define CONFFILEWIDGET_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QtWidgets>
#pragma GCC diagnostic pop

#include <fstream>
#include <memory>

#include "ConfBlockWidget.hpp"
#include "ConfFile.hpp"

class ConfFileWidget : public QWidget
{
  Q_OBJECT

public:
  ConfFileWidget(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile, QWidget* parent = 0);
  ConfFileWidget(const ConfFileWidget&) = delete;
  ConfFileWidget& operator=(const ConfFileWidget&) = delete;
  virtual ~ConfFileWidget();

  void setDebug(bool state);
  void updateTabs();
  void clear();

  void nextTab(int step);
  void setTab(const std::string& tabName);

  ConfFile& getConfFile();

private slots:
  void tabSelected();

private:
  ConfFile _confFile;
  bool _isDebug;

  std::string _filename;
  std::vector<ConfBlockWidget*> _confBlockWidgets;
  QTabWidget* _tabWidget;
  QVBoxLayout* _layout;
};

#endif
