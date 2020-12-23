#include "BSONDocWidget.hpp"

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
  _tableWidget->blockSignals(false);
}

void BSONDocWidget::setDebug(bool /*state*/)
{
}
