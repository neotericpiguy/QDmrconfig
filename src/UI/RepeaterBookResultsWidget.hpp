#ifndef REPEATERBOOKRESULTSWIDGET_HPP
#define REPEATERBOOKRESULTSWIDGET_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QtWidgets>
#include <memory>
#pragma GCC diagnostic pop

#include <iostream>

#include "BSONDoc.hpp"

class RepeaterBookResultsWidget : public QWidget
{
  Q_OBJECT

public:
  RepeaterBookResultsWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent = 0);
  RepeaterBookResultsWidget(const RepeaterBookResultsWidget&) = delete;
  RepeaterBookResultsWidget& operator=(const RepeaterBookResultsWidget&) = delete;
  virtual ~RepeaterBookResultsWidget();

  void update();
  void metaUpdate();
  void setDebug(bool state);
  void updateTable();
  std::vector<Mongo::BSONDoc> getVisibleDocs() const;

private slots:
  void itemUpdate(QTableWidgetItem* item);
  void cellSelected(int nRow, int nCol);
  void duplicateTableRow();
  void removeTableRow();
  void filterTableColumn();
  void hideRow();
  void sortTableRow();
  void removeValueAction();
  void addValueAction();

private:
  bool _isDebug;
  std::vector<Mongo::BSONDoc> _bsonDocs;

  //  QPlainTextEdit* _textView;
  QTableWidget* _tableWidget;
};

#endif
