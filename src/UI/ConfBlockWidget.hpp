#ifndef CONFBLOCKWIDGET_H
#define CONFBLOCKWIDGET_H

#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <memory>

#include "ConfBlock.hpp"
#include "ConfFile.hpp"

class ConfBlockWidget : public QWidget
{
  Q_OBJECT

public:
  ConfBlockWidget(ConfBlock& confBlock, QWidget* parent = 0);
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

private:
  bool _isDebug;
  ConfBlock& _confBlock;

  QPlainTextEdit* _textView;
  QTableWidget* _tableWidget;
};

#endif
