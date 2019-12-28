#ifndef CONFBLOCKWIDGET_H
#define CONFBLOCKWIDGET_H

#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <memory>

#include "ConfBlock.hpp"

class ConfBlockWidget : public QWidget
{
  Q_OBJECT

public:
  ConfBlockWidget(ConfBlock& confBlock);
  virtual ~ConfBlockWidget();

  void update();
  void setDebug(bool state);

private slots:
  void itemUpdate(QTableWidgetItem* item);
  void cellSelected(int nRow, int nCol);
  void duplicateTableRow();
  void removeTableRow();

private:
  void metaUpdate();

  bool isDebug;
  ConfBlock& _confBlock;
  std::unique_ptr<QPlainTextEdit> _textView;
  std::unique_ptr<QTableWidget> _tableWidget;
};

#endif
