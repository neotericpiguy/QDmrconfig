#ifndef CONFBLOCKWIDGET_H
#define CONFBLOCKWIDGET_H

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QtWidgets>
#include <memory>
#pragma GCC diagnostic pop

#include "ConfBlock.hpp"
#include "ConfFile.hpp"

class ConfBlockWidget : public QWidget
{
  Q_OBJECT

public:
  ConfBlockWidget(ConfBlock& confBlock, QWidget* parent = 0);
  ConfBlockWidget(const ConfBlockWidget&) = delete;
  ConfBlockWidget& operator=(const ConfBlockWidget&) = delete;
  virtual ~ConfBlockWidget();

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
  ConfBlock& _confBlock;

  QPlainTextEdit* _textView;
  QTableWidget* _tableWidget;
};

#endif
