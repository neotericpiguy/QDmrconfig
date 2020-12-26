#ifndef WIDGETTESTS_HPP
#define WIDGETTESTS_HPP

#include <QMainWindow>
#include <QtWidgets>

#include "BSONDocWidget.hpp"
#include "BSONDocTests.hpp"
#include "SimpleTest.hpp"

class WidgetTests : public QMainWindow
{
  Q_OBJECT

public:
  WidgetTests();
  WidgetTests(const WidgetTests&) = delete;
  ~WidgetTests();

  bool initBsonDocWidget();

private:
  QTabWidget* _tabWidget;
  QVBoxLayout* _layout;

  // BSONDocWidget
  std::vector<Mongo::BSONDoc> bsonDocs;
  BSONDocWidget* bsonDocWidget;
};

#endif
