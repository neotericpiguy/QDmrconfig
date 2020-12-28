#include "BSONDocWidget.hpp"

#include <regex>

BSONDocWidget::BSONDocWidget(std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent) :
    QWidget(parent),
    _isDebug(false),
    _bsonDocs(bsonDocs),
    _tableWidget(new QTableWidget(this))
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(_tableWidget);
  setLayout(layout);

  // Fit contents of table cells
  _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  // Lights up the right click ability
  setContextMenuPolicy(Qt::ActionsContextMenu);

  QAction* filterAction = new QAction("&Filter Row by");
  filterAction->setShortcut(QKeySequence(tr("Ctrl+F")));
  addAction(filterAction);
  connect(filterAction, &QAction::triggered, this, [this]() { filterTableColumn(); });

  QAction* hideAction = new QAction("&Hide row");
  hideAction->setShortcut(QKeySequence(tr("Ctrl+h")));
  addAction(hideAction);
  connect(hideAction, &QAction::triggered, this, [this]() { hideRow(); });
}

BSONDocWidget::~BSONDocWidget()
{
}

void BSONDocWidget::duplicateTableRow()
{
}

void BSONDocWidget::removeTableRow()
{
}

void BSONDocWidget::sortTableRow()
{
}

void BSONDocWidget::filterTableColumn()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Filter Column by String"),
                                       tr("Filter Column by  String"), QLineEdit::Normal,
                                       "", &ok);

  if (!ok)
    return;

  for (int i = 0; _tableWidget->selectedItems().empty() && i < _tableWidget->rowCount(); i++)
    _tableWidget->showRow(i);

  for (const auto& item : _tableWidget->selectedItems())
  {
    int j = item->column();
    for (int i = 0; i < _tableWidget->rowCount(); i++)
    {
      std::string cellText = _tableWidget->item(i, j)->text().toStdString();

      try
      {
        std::regex e(".*" + text.toStdString() + ".*", std::regex_constants::icase);

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

void BSONDocWidget::hideRow()
{
  for (const auto& item : _tableWidget->selectedItems())
  {
    _tableWidget->hideRow(item->column());
  }
}

void BSONDocWidget::removeValueAction()
{
}

void BSONDocWidget::addValueAction()
{
}

void BSONDocWidget::itemUpdate(QTableWidgetItem* /*item*/)
{
}

void BSONDocWidget::metaUpdate()
{
}

void BSONDocWidget::updateTable()
{
}

void BSONDocWidget::cellSelected(int, int)
{
}

void BSONDocWidget::update()
{
  if (_bsonDocs.empty())
    return;

  _tableWidget->blockSignals(true);
  _tableWidget->clear();

  std::vector<std::string> keys = _bsonDocs.at(0).getKeys();

  _tableWidget->setRowCount(_bsonDocs.size());
  _tableWidget->setColumnCount(keys.size());

  // Set header info *has to be done after setting the row and column size
  QStringList headers;
  for (const auto& key : keys)
    headers << key.c_str();
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
  _tableWidget->blockSignals(false);
}

void BSONDocWidget::setDebug(bool /*state*/)
{
}
