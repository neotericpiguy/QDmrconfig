#include "ConfBlockWidget.hpp"

ConfBlockWidget::ConfBlockWidget(ConfBlock& confBlock, QWidget* parent) :
    QWidget(parent),
    _isDebug(false),
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
  QAction* filterAction = new QAction("&Filter Row by");
  filterAction->setShortcut(QKeySequence(tr("Ctrl+F")));
  addAction(filterAction);

  QAction* duplicateAction = new QAction("&Duplicate Row");
  addAction(duplicateAction);

  QAction* removeAction = new QAction("&Remove Row");
  addAction(removeAction);

  QAction* sortAction = new QAction("&Sort Rows");
  addAction(sortAction);

  connect(duplicateAction, SIGNAL(triggered()), this, SLOT(duplicateTableRow()));
  connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTableRow()));
  connect(filterAction, SIGNAL(triggered()), this, SLOT(filterTableColumn()));
  connect(sortAction, SIGNAL(triggered()), this, SLOT(sortTableRow()));
}

ConfBlockWidget::~ConfBlockWidget()
{
}

void ConfBlockWidget::duplicateTableRow()
{
  if (_tableWidget->selectedItems().empty())
    return;

  if (_isDebug)
    qDebug() << _tableWidget->currentItem()->text();

  int selectedRow = _tableWidget->selectedItems()[0]->row();

  for (const auto& item : _tableWidget->selectedItems())
  {
    int row = item->row();
    _confBlock.insertRow(row, _confBlock.getRow(row));
  }

  _confBlock.setModified(true);
  update();

  _tableWidget->selectRow(selectedRow);
}

void ConfBlockWidget::removeTableRow()
{
  if (_isDebug)
    qDebug() << _tableWidget->currentItem()->text();

  static std::vector<std::string> empty;
  for (const auto& item : _tableWidget->selectedItems())
    _confBlock.getRow(item->row()) = empty;

  auto iter = _confBlock.getRows().begin();

  auto removeIter = std::remove_if(iter, _confBlock.getRows().end(), [&](const std::vector<std::string>& val) {
    return (val == empty);
  });

  _confBlock.getRows().erase(removeIter, _confBlock.getRows().end());

  _confBlock.setModified(true);
  update();
}

void ConfBlockWidget::sortTableRow()
{
  if (_isDebug)
    qDebug() << _tableWidget->currentItem()->text();

  if (_tableWidget->selectedItems().empty())
    return;

  std::vector<std::vector<std::string>> vecsMap;
  std::vector<int> original;

  int columnIndex = _tableWidget->selectedItems()[0]->column();

  for (const auto& item : _tableWidget->selectedItems())
  {
    vecsMap.push_back(_confBlock.getRow(item->row()));
    original.push_back(item->row());
  }

  std::sort(vecsMap.begin(), vecsMap.end(), [columnIndex](std::vector<std::string> a, std::vector<std::string> b) {
    return a[columnIndex] < b[columnIndex];
  });

  std::sort(original.begin(), original.end());
  std::unique(original.begin(), original.end());

  int i = 0;
  for (const auto& value : vecsMap)
  {
    _confBlock.getRow(original[i]) = value;
    i++;
  }

  _confBlock.setModified(true);
  update();
}

void ConfBlockWidget::filterTableColumn()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Filter Column by String"),
                                       tr("Filter Column by  String"), QLineEdit::Normal,
                                       "", &ok);

  if (!ok)
    return;

  if (_tableWidget->selectedItems().empty())
  {
    for (int i = 0; i < _tableWidget->rowCount(); i++)
    {
      _tableWidget->showRow(i);
    }
  }

  for (const auto& item : _tableWidget->selectedItems())
  {
    int j = item->column();
    for (int i = 0; i < _tableWidget->rowCount(); i++)
    {
      std::string cellText = _tableWidget->item(i, j)->text().toStdString();

      try
      {
        std::regex e(text.toStdString());

        if (text == "")
          _tableWidget->showRow(i);
        else if (!std::regex_match(cellText, e))
          _tableWidget->hideRow(i);
      }
      catch (const std::exception& e)
      {
        //Msg box error to user
      }
    }
  }
}

void ConfBlockWidget::itemUpdate(QTableWidgetItem* item)
{
  bool needMetaUpdate = false;
  auto newValue = item->text().toStdString();
  newValue.erase(std::remove_if(newValue.begin(), newValue.end(), isspace), newValue.end());
  ConfBlock::replace(newValue, " ", "_");

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

void ConfBlockWidget::cellSelected(int, int)
{
  QMessageBox::information(this, "", "asdlfasjkfd");
}

void ConfBlockWidget::update()
{
  _tableWidget->blockSignals(true);
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
  _tableWidget->blockSignals(false);
}

void ConfBlockWidget::setDebug(bool state)
{
  _isDebug = state;
  _textView->setVisible(_isDebug);
}
