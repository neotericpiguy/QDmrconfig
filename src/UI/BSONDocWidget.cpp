#include "BSONDocWidget.hpp"

BSONDocWidget::BSONDocWidget(std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent) :
    QWidget(parent),
    _isDebug(false),
    _bsonDocs(bsonDocs),
    _tableWidget(new QTableWidget(this))
{
  QVBoxLayout* layout = new QVBoxLayout;
  //  layout->addWidget(_textView);
  layout->addWidget(_tableWidget);
  setLayout(layout);

  //  QFont f("unexistent");
  //  f.setStyleHint(QFont::Monospace);
  //  _textView->setFont(f);
  //
  //  _textView->setReadOnly(true);
  //  _textView->setPlainText(_bsonDoc.getConfLines(false).c_str());

  //update();

  //_tableWidget->verticalHeader()->setVisible(false);

  // Fit contents of table cells
  _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  //
  //  connect(_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemUpdate(QTableWidgetItem*)));
  //
  // setContextMenuPolicy(Qt::ActionsContextMenu);
  //
  //  QAction* filterAction = new QAction("&Filter Row by");
  //  filterAction->setShortcut(QKeySequence(tr("Ctrl+F")));
  //  addAction(filterAction);
  //
  //  QAction* duplicateAction = new QAction("Dupl&icate Row");
  //  addAction(duplicateAction);
  //
  //  QAction* removeAction = new QAction("&Delete Row");
  //  addAction(removeAction);
  //
  //  QAction* sortAction = new QAction("&Sort Rows");
  //  addAction(sortAction);
  //
  //  QAction* removeValueAction = new QAction("&Remove value");
  //  addAction(removeValueAction);
  //
  //  QAction* addValueAction = new QAction("&Add value");
  //  addAction(addValueAction);
  //
  //  connect(duplicateAction, SIGNAL(triggered()), this, SLOT(duplicateTableRow()));
  //  connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTableRow()));
  //  connect(filterAction, SIGNAL(triggered()), this, SLOT(filterTableColumn()));
  //  connect(sortAction, SIGNAL(triggered()), this, SLOT(sortTableRow()));
  //  connect(removeValueAction, SIGNAL(triggered()), this, SLOT(removeValueAction()));
  //  connect(addValueAction, SIGNAL(triggered()), this, SLOT(addValueAction()));
}

BSONDocWidget::~BSONDocWidget()
{
}

void BSONDocWidget::duplicateTableRow()
{
  //  if (_tableWidget->selectedItems().empty())
  //    return;
  //
  //  if (_isDebug)
  //    qDebug() << _tableWidget->currentItem()->text();
  //
  //  int selectedRow = _tableWidget->selectedItems()[0]->row();
  //
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    int row = item->row();
  //    _bsonDoc.insertRow(row, _bsonDoc.getRow(row));
  //  }
  //
  //  _bsonDoc.setModified(true);
  //  update();
  //
  //  _tableWidget->selectRow(selectedRow);
}

void BSONDocWidget::removeTableRow()
{
  //  if (_isDebug)
  //    qDebug() << _tableWidget->currentItem()->text();
  //
  //  static std::vector<std::string> empty;
  //  for (const auto& item : _tableWidget->selectedItems())
  //    _bsonDoc.getRow(item->row()) = empty;
  //
  //  auto iter = _bsonDoc.getRows().begin();
  //
  //  auto removeIter = std::remove_if(iter, _bsonDoc.getRows().end(), [&](const std::vector<std::string>& val) {
  //    return (val == empty);
  //  });
  //
  //  _bsonDoc.getRows().erase(removeIter, _bsonDoc.getRows().end());
  //
  //  _bsonDoc.setModified(true);
  //  update();
}

void BSONDocWidget::sortTableRow()
{
  //  if (_isDebug)
  //    qDebug() << _tableWidget->currentItem()->text();
  //
  //  if (_tableWidget->selectedItems().empty())
  //    return;
  //
  //  std::vector<std::vector<std::string>> vecsMap;
  //  std::vector<int> original;
  //
  //  int columnIndex = _tableWidget->selectedItems()[0]->column();
  //
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    vecsMap.push_back(_bsonDoc.getRow(item->row()));
  //    original.push_back(item->row());
  //  }
  //
  //  std::sort(vecsMap.begin(), vecsMap.end(), [columnIndex](std::vector<std::string> a, std::vector<std::string> b) {
  //    return a[columnIndex] < b[columnIndex];
  //  });
  //
  //  std::sort(original.begin(), original.end());
  //  std::unique(original.begin(), original.end());
  //
  //  int i = 0;
  //  for (const auto& value : vecsMap)
  //  {
  //    _bsonDoc.getRow(original[i]) = value;
  //    i++;
  //  }
  //
  //  _bsonDoc.setModified(true);
  //  update();
}

void BSONDocWidget::filterTableColumn()
{
  //  bool ok;
  //  QString text = QInputDialog::getText(this, tr("Filter Column by String"),
  //                                       tr("Filter Column by  String"), QLineEdit::Normal,
  //                                       "", &ok);
  //
  //  if (!ok)
  //    return;
  //
  //  for (int i = 0; _tableWidget->selectedItems().empty() && i < _tableWidget->rowCount(); i++)
  //    _tableWidget->showRow(i);
  //
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    int j = item->column();
  //    for (int i = 0; i < _tableWidget->rowCount(); i++)
  //    {
  //      std::string cellText = _tableWidget->item(i, j)->text().toStdString();
  //
  //      try
  //      {
  //        std::regex e(".*" + text.toStdString() + ".*", std::regex_constants::icase);
  //
  //        if (text == "")
  //          _tableWidget->showRow(i);
  //        else if (!std::regex_match(cellText, e))
  //          _tableWidget->hideRow(i);
  //      }
  //      catch (const std::exception& e)
  //      {
  //        //Msg box error to user
  //      }
  //    }
  //  }
}

void BSONDocWidget::removeValueAction()
{
  //  bool ok;
  //  QString text = QInputDialog::getText(this, tr("Value to remove"),
  //                                       tr("Value to remove"), QLineEdit::Normal,
  //                                       "", &ok);
  //
  //  if (!ok)
  //    return;
  //
  //  std::string textToRemove = text.toStdString();
  //
  //  _tableWidget->blockSignals(true);
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    //    std::string cellText = _tableWidget->item(i, j)->text().toStdString();
  //    std::string cellText = item->text().toStdString();
  //
  ////    BSONDoc::replaceRegex(cellText, "^" + textToRemove + "$", "");
  ////    BSONDoc::replaceRegex(cellText, "^" + textToRemove + ",", "");
  ////    BSONDoc::replaceRegex(cellText, "," + textToRemove + ",", ",");
  ////    BSONDoc::replaceRegex(cellText, "," + textToRemove + "(,|$)", "");
  //    if(cellText == "")
  //      cellText = "-";
  //    item->setText(cellText.c_str());
  //    _bsonDoc.getRows()[item->row()][item->column()] = cellText;
  //  }
  //  _tableWidget->blockSignals(false);
}

void BSONDocWidget::addValueAction()
{
  //  bool ok;
  //  QString text = QInputDialog::getText(this, tr("Value to add"),
  //                                       tr("Value to add"), QLineEdit::Normal,
  //                                       "", &ok);
  //
  //  if (!ok)
  //    return;
  //
  //  QString textToAdd(text);
  //
  //  _tableWidget->blockSignals(true);
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    auto cellText = item->text();
  //
  //    cellText.append(",");
  //    cellText.append(textToAdd);
  //
  //    if (cellText[0] == ',')
  //      cellText = cellText.right(cellText.length() - 1);
  //
  //    item->setText(cellText);
  //    _bsonDoc.getRows()[item->row()][item->column()] = cellText.toStdString();
  //  }
  //  _tableWidget->blockSignals(false);
}

void BSONDocWidget::itemUpdate(QTableWidgetItem* /*item*/)
{
  //  bool needMetaUpdate = false;
  //  auto newValue = item->text().toStdString();
  //  newValue.erase(std::remove_if(newValue.begin(), newValue.end(), isspace), newValue.end());
  ////  BSONDoc::replace(newValue, " ", "_");
  //
  //  for (const auto& item : _tableWidget->selectedItems())
  //  {
  //    int row = item->row();
  //    int column = item->column();
  //    if (_bsonDoc.isTable())
  //    {
  //      auto& rows = _bsonDoc.getRows();
  //      rows[row][column] = newValue;
  //      item->setText(newValue.c_str());
  //
  //      if (column > _bsonDoc.getMetaIndex())
  //        needMetaUpdate = true;
  //    }
  //    else
  //    {
  //      auto& valueMap = _bsonDoc.getMap();
  //      auto vi = valueMap.begin();
  //      std::advance(vi, row);
  //
  //      // if Value changed
  //      if (column == 1)
  //      {
  //        vi->second = newValue;
  //      }
  //      else  // don't let them change the key
  //      {
  //        item->setText(vi->first.c_str());
  //      }
  //    }
  //  }
  //
  //  if (needMetaUpdate)
  //    metaUpdate();
  //
  //  _bsonDoc.setModified(true);
  //  if (_isDebug)
  //    qDebug() << "Modified";
  //  _textView->setPlainText(_bsonDoc.getConfLines(false).c_str());
}

void BSONDocWidget::metaUpdate()
{
  //  if (_bsonDoc.isTable())
  //  {
  //    if (_isDebug)
  //      qDebug() << _bsonDoc.getHeader().c_str() << " meta update";
  //    _bsonDoc.metaUpdate();
  //
  //    updateTable();
  //  }
  //  _textView->setPlainText(_bsonDoc.getConfLines(false).c_str());
}

void BSONDocWidget::updateTable()
{
  const std::vector<std::string> keys = {"callsign", "licName", "frn", "expiredDate"};
  _tableWidget->blockSignals(true);
  qDebug() << "rows: " << _bsonDocs.size() << "," << _tableWidget->rowCount();
  qDebug() << "columns: " << keys.size() << "," << _tableWidget->columnCount();
  //_tableWidget->setRowCount(_bsonDocs.size());
  //_tableWidget->setColumnCount(keys.size());
  //  for (uint16_t row = 0; row < _bsonDocs.size(); ++row)
  //  {
  //    const auto& doc = _bsonDocs.at(row);
  //    std::cout << doc.toString() << std::endl;
  //    for (uint16_t column = 0; column < keys.size(); ++column)
  //    {
  //      const auto& key = keys.at(column);
  //      if (doc.has(key))
  //      {
  //        //        _tableWidget->setItem(row, column, new QTableWidgetItem(doc.get<std::string>(key).c_str()));
  //        //        _tableWidget->item(i, j)->setText(rows[i][j].c_str());
  //      }
  //    }
  //  }
  //  const auto& rows = _bsonDoc.getRows();
  //
  //  int modifiedCells = 0;
  //
  //  if (_isDebug)
  //  {
  //    qDebug() << "rows: " << _bsonDoc.getRowCount() << "," << _tableWidget->rowCount();
  //    qDebug() << "columns: " << _bsonDoc.getColumnCount() << "," << _tableWidget->columnCount();
  //  }
  //
  //  if ((int)_bsonDoc.getRowCount() != _tableWidget->rowCount())
  //    update();
  //
  //  for (unsigned int i = 0; i < _bsonDoc.getRowCount(); i++)
  //  {
  //    for (unsigned int j = 0; j < _bsonDoc.getColumnCount(); j++)
  //    {
  //      if (_tableWidget->item(i, j)->text().toStdString() != rows[i][j])
  //      {
  //        _tableWidget->item(i, j)->setText(rows[i][j].c_str());
  //        modifiedCells++;
  //      }
  //    }
  //  }
  //
  //  if (_isDebug)
  //    qDebug() << "Modified cells: " << modifiedCells;
  _tableWidget->blockSignals(false);
}

void BSONDocWidget::cellSelected(int, int)
{
  //  QMessageBox::information(this, "", "asdlfasjkfd");
}

void BSONDocWidget::update()
{
  _tableWidget->blockSignals(true);
  _tableWidget->clear();

  const std::vector<std::string> keys = {"callsign", "licName", "frn", "expiredDate"};
  _tableWidget->setRowCount(_bsonDocs.size());
  _tableWidget->setColumnCount(keys.size());
  QStringList headers;
  for (uint16_t column = 0; column < keys.size(); ++column)
  {
    headers << keys.at(column).c_str();
  }
  _tableWidget->setHorizontalHeaderLabels(headers);
  for (uint16_t row = 0; row < _bsonDocs.size(); ++row)
  {
    const auto& doc = _bsonDocs.at(row);
    for (uint16_t column = 0; column < keys.size(); ++column)
    {
      const auto& key = keys.at(column);
      if (doc.has(key))
      {
        _tableWidget->setItem(row, column, new QTableWidgetItem(doc.get<std::string>(key).c_str()));
      }
    }
  }
  //  QStringList headers;
  //  if (_bsonDoc.isTable())
  //  {
  //    _tableWidget->setRowCount(_bsonDoc.getRowCount());
  //    _tableWidget->setColumnCount(_bsonDoc.getColumnCount());
  //
  //    for (const auto& c : _bsonDoc.getColumnNames())
  //      headers << c.c_str();
  //
  //    const auto& lines = _bsonDoc.getRows();
  //    for (unsigned int i = 0; i < _bsonDoc.getRowCount(); i++)
  //    {
  //      const auto& line = lines[i];
  //      for (unsigned int j = 0; j < _bsonDoc.getColumnCount(); j++)
  //      {
  //        _tableWidget->setItem(i, j, new QTableWidgetItem(line[j].c_str()));
  //      }
  //    }
  //  }
  //  else
  //  {
  //    // BSONDoc is a map
  //    _tableWidget->setRowCount(_bsonDoc.getMap().size());
  //    _tableWidget->setColumnCount(2);
  //
  //    headers << "Key";
  //    headers << "Value";
  //
  //    int row = 0;
  //    for (const auto& [key, value] : _bsonDoc.getMap())
  //    {
  //      _tableWidget->setItem(row, 0, new QTableWidgetItem(key.c_str()));
  //      _tableWidget->setItem(row++, 1, new QTableWidgetItem(value.c_str()));
  //    }
  //  }
  //  _tableWidget->setHorizontalHeaderLabels(headers);
  _tableWidget->blockSignals(false);
}

void BSONDocWidget::setDebug(bool /*state*/)
{
  //  _isDebug = state;
  //  _textView->setVisible(_isDebug);
}
