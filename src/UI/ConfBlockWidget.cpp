#include "ConfBlockWidget.hpp"

#include <QtWidgets/QtWidgets>

ConfBlockWidget::ConfBlockWidget(ConfBlock& confBlock) :
    QWidget(),
    _confBlock(confBlock),
    _textView(new QPlainTextEdit()),
    _tableWidget(new QTableWidget())
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(_textView.get());
  layout->addWidget(_tableWidget.get());
  setLayout(layout);

  QFont f("unexistent");
  f.setStyleHint(QFont::Monospace);
  _textView->setFont(f);

  _textView->setReadOnly(true);
  _textView->setPlainText(_confBlock.getConfLines(false).c_str());

  update();

  _tableWidget->verticalHeader()->setVisible(false);

  // Fit contents of table cells
  _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  connect(_tableWidget.get(), SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemUpdate(QTableWidgetItem*)));

  setContextMenuPolicy(Qt::ActionsContextMenu);
  QAction* duplicateAction = new QAction("Duplicate Row");
  QAction* removeAction = new QAction("Remove Row");
  connect(duplicateAction, SIGNAL(triggered()), this, SLOT(duplicateTableRow()));
  connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTableRow()));
  addAction(duplicateAction);
  addAction(removeAction);
}

ConfBlockWidget::~ConfBlockWidget()
{
}

void ConfBlockWidget::duplicateTableRow()
{
  qDebug() << _tableWidget->currentItem()->text();

  int row = _tableWidget->currentRow();
  _confBlock.insertRow(row, _confBlock.getRow(row));

  update();
}

void ConfBlockWidget::removeTableRow()
{
  qDebug() << _tableWidget->currentItem()->text();

  int row = _tableWidget->currentRow();
  _confBlock.removeRow(row);

  update();
}

void ConfBlockWidget::itemUpdate(QTableWidgetItem* item)
{
  auto newValue = item->text().toStdString();
  for (const auto& item : _tableWidget->selectedItems())
  {
    int row = item->row();
    int column = item->column();
    if (_confBlock.isTable())
    {
      auto& rows = _confBlock.getRows();
      rows[row][column] = newValue;
      item->setText(newValue.c_str());
    }
    else
    {
      auto& valueMap = _confBlock.getMap();
      auto vi = valueMap.begin();
      std::advance(vi, row);

      // if Value changed
      if (column == 1)
      {
        vi->second = newValue;
      }
      else  // don't let them change the key
      {
        item->setText(vi->first.c_str());
      }
    }
  }

  _confBlock.setModified(true);

  _textView->setPlainText(_confBlock.getConfLines(false).c_str());
}

void ConfBlockWidget::cellSelected(int nRow, int nCol)
{
  QMessageBox::information(this, "", "asdlfasjkfd");
}

void ConfBlockWidget::update()
{
  _tableWidget->clear();
  QStringList headers;
  if (_confBlock.isTable())
  {
    _tableWidget->setRowCount(_confBlock.getRowCount());
    _tableWidget->setColumnCount(_confBlock.getColumnCount());

    for (const auto& c : _confBlock.getColumnNames())
      headers << c.c_str();

    const auto& lines = _confBlock.getRows();
    for (unsigned int i = 0; i < _confBlock.getRowCount(); i++)
    {
      const auto& line = lines[i];
      for (unsigned int j = 0; j < _confBlock.getColumnCount(); j++)
      {
        _tableWidget->setItem(i, j, new QTableWidgetItem(line[j].c_str()));
      }
    }
  }
  else
  {
    // ConfBlock is a map
    _tableWidget->setRowCount(_confBlock.getMap().size());
    _tableWidget->setColumnCount(2);

    headers << "Key";
    headers << "Value";

    int row = 0;
    for (const auto& [key, value] : _confBlock.getMap())
    {
      _tableWidget->setItem(row, 0, new QTableWidgetItem(key.c_str()));
      _tableWidget->setItem(row++, 1, new QTableWidgetItem(value.c_str()));
    }
  }
  _tableWidget->setHorizontalHeaderLabels(headers);
}
