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
  BSONDocWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent = 0);
  BSONDocWidget(const BSONDocWidget&) = delete;
  BSONDocWidget& operator=(const BSONDocWidget&) = delete;
  virtual ~BSONDocWidget();

  void update();
  void setDebug(bool state);
  std::vector<Mongo::BSONDoc> getVisibleDocs() const;

  void filterTableColumn();
  void hideRow();

private:
  std::vector<Mongo::BSONDoc> _bsonDocs;

  //  QPlainTextEdit* _textView;
  QTableWidget* _tableWidget;
};

#endif
