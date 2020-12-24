#ifndef BSONDOCWIDGET_HPP
#define BSONDOCWIDGET_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QtWidgets>
#include <memory>
#pragma GCC diagnostic pop

#include <iostream>

#include "BSONDoc.hpp"

class BSONDocWidget : public QWidget
{
  Q_OBJECT

public:
  BSONDocWidget(std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent = 0);
  BSONDocWidget(const BSONDocWidget&) = delete;
  BSONDocWidget& operator=(const BSONDocWidget&) = delete;
  virtual ~BSONDocWidget();

  void update();
  void metaUpdate();
  void setDebug(bool state);
  void updateTable();

private slots:
  void itemUpdate(QTableWidgetItem* item);
  void cellSelected(int nRow, int nCol);
  void duplicateTableRow();
  void removeTableRow();
  void filterTableColumn();
  void sortTableRow();
  void removeValueAction();
  void addValueAction();

private:
  bool _isDebug;
  std::vector<Mongo::BSONDoc>& _bsonDocs;

  //  QPlainTextEdit* _textView;
  QTableWidget* _tableWidget;
};

#endif
