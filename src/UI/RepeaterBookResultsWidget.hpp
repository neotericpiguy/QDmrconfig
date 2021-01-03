#ifndef REPEATERBOOKRESULTSWIDGET_HPP
#define REPEATERBOOKRESULTSWIDGET_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QtWidgets>
#include <memory>
#pragma GCC diagnostic pop

#include "BSONDocWidget.hpp"

class RepeaterBookResultsWidget : public BSONDocWidget
{
  Q_OBJECT

public:
  RepeaterBookResultsWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent = 0);
  RepeaterBookResultsWidget(const RepeaterBookResultsWidget&) = delete;
  RepeaterBookResultsWidget& operator=(const RepeaterBookResultsWidget&) = delete;
  virtual ~RepeaterBookResultsWidget();

  void filterVhfUhfFrequencies();
};

#endif
