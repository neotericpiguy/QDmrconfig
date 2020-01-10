#include "ConfBlockWidget.hpp"

#include <QtWidgets/QtWidgets>

ConfBlockWidget::ConfBlockWidget(ConfBlock& confBlock, QWidget* parent) :
    QWidget(parent),
    _confBlock(confBlock),
    _textView(new QPlainTextEdit(this)),
    _tableWidget(new QTableWidget(this))
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(_textView);
  layout->addWidget(_tableWidget);
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

  connect(_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemUpdate(QTableWidgetItem*)));

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
  if (_isDebug)
    qDebug() << _tableWidget->currentItem()->text();

  int row = _tableWidget->currentRow();
  _confBlock.insertRow(row, _confBlock.getRow(row));

  update();
}

void ConfBlockWidget::removeTableRow()
{
  if (_isDebug)
    qDebug() << _tableWidget->currentItem()->text();

  static std::vector<std::string> empty;
  for (const auto& item : _tableWidget->selectedItems())
  {
    //    _confBlock.removeRow(item->row());
    _confBlock.getRow(item->row()) = empty;
  }

  auto iter = _confBlock.getRows().begin();

  auto removeIter = std::remove_if(iter, _confBlock.getRows().end(), [&](const std::vector<std::string>& val) {
    return (val == empty);
  });

  _confBlock.getRows().erase(removeIter, _confBlock.getRows().end());

  update();
}

void ConfBlockWidget::itemUpdate(QTableWidgetItem* item)
{
  bool needMetaUpdate = false;
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

      if (column > _confBlock.getMetaIndex())
        needMetaUpdate = true;
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

  if (needMetaUpdate)
    metaUpdate();

  _confBlock.setModified(true);
  if (_isDebug)
    qDebug() << "Modified";
  _textView->setPlainText(_confBlock.getConfLines(false).c_str());
}

void ConfBlockWidget::metaUpdate()
{
  if (_confBlock.isTable())
  {
    if (_isDebug)
      qDebug() << _confBlock.getHeader().c_str() << " meta update";
    _confBlock.metaUpdate();

    updateTable();
  }
  _textView->setPlainText(_confBlock.getConfLines(false).c_str());
}

void ConfBlockWidget::updateTable()
{
  _tableWidget->blockSignals(true);
  const auto& rows = _confBlock.getRows();

  int modifiedCells = 0;

  if (_isDebug)
  {
    qDebug() << "rows: " << _confBlock.getRowCount() << "," << _tableWidget->rowCount();
    qDebug() << "columns: " << _confBlock.getColumnCount() << "," << _tableWidget->columnCount();
  }

  for (unsigned int i = 0; i < _confBlock.getRowCount(); i++)
  {
    for (unsigned int j = 0; j < _confBlock.getColumnCount(); j++)
    {
      if (_tableWidget->item(i, j)->text().toStdString() != rows[i][j])
      {
        _tableWidget->item(i, j)->setText(rows[i][j].c_str());
        modifiedCells++;
      }
    }
  }

  if (_isDebug)
    qDebug() << "Modified cells: " << modifiedCells;
  _tableWidget->blockSignals(false);
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

void ConfBlockWidget::setDebug(bool state)
{
  _isDebug = state;
  if (_isDebug)
  {
    _textView->setVisible(true);
  }
  else
  {
    _textView->setVisible(false);
  }
}
