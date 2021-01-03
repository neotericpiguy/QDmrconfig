#include "RepeaterBookResultsWidget.hpp"

#include <regex>

RepeaterBookResultsWidget::RepeaterBookResultsWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent) :
    BSONDocWidget(bsonDocs, parent)
{
  auto leftLayout = new QHBoxLayout;

  auto vhfUhfPushButton = new QPushButton("VHF/UHF");
  connect(vhfUhfPushButton, &QPushButton::released, this, [](this) { filterVhfUhfFrequencies(); });
  leftLayout->addWidget(vhfUhfPushButton);

  _layout->insertLayout(0, leftLayout);
}

RepeaterBookResultsWidget::~RepeaterBookResultsWidget()
{
}

void RepeaterBookResultsWidget::filterVhfUhfFrequencies()
{
}
