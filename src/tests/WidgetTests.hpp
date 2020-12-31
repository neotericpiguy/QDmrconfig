#ifndef WIDGETTESTS_HPP
#define WIDGETTESTS_HPP

#include <QMainWindow>
#include <QtWidgets>

#include "BSONDocTests.hpp"
#include "BSONDocWidget.hpp"
#include "ConfFileWidget.hpp"
#include "FieldEntryDialog.hpp"
#include "SimpleTest.hpp"

class WidgetTests : public QMainWindow
{
  Q_OBJECT

public:
  WidgetTests();
  WidgetTests(const WidgetTests&) = delete;
  ~WidgetTests();

  bool initBsonDocWidget();
  bool initConfFileWidget();
  bool initFieldEntryDialog();
  bool initChirpCsvTests();

private:
  QTabWidget* _tabWidget;
  QVBoxLayout* _layout;

  // ConfFileWidget
  //  ConfFileWidget* _confFileWidget;
};

#endif
