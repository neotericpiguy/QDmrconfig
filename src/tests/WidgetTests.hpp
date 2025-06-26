#ifndef WIDGETTESTS_HPP
#define WIDGETTESTS_HPP

#include <QMainWindow>
#include <QtWidgets>

class WidgetTests : public QMainWindow
{
  Q_OBJECT

public:
  WidgetTests();
  WidgetTests(const WidgetTests&) = delete;
  ~WidgetTests();

  bool initFieldEntryDialog();

  bool initBsonConfFileWidget();
  bool initBsonDocWidget();
  bool repeaterBookExport();
  bool initChirpCsvTests();
  bool initRepeaterBookSearchResultsWidget();

private:
  QTabWidget* _tabWidget;
  QVBoxLayout* _layout;
};

#endif
