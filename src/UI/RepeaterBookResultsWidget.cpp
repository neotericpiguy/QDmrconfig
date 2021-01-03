#include "RepeaterBookResultsWidget.hpp"

#include <regex>

RepeaterBookResultsWidget::RepeaterBookResultsWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent) :
    BSONDocWidget(bsonDocs, parent)
{
  auto leftLayout = new QHBoxLayout;

  auto showAllPushButton = new QPushButton("All");
  connect(showAllPushButton, &QPushButton::released, this, [this]() {
    filterPreset("Frequency", "");
  });
  leftLayout->addWidget(showAllPushButton);

  auto vhfUhfPushButton = new QPushButton("VHF/UHF");
  connect(vhfUhfPushButton, &QPushButton::released, this, [this]() {
    filterPreset("Frequency", "^(1|4)\\d\\d\\.");
  });
  leftLayout->addWidget(vhfUhfPushButton);

  auto usePushButton = new QPushButton("OPEN");
  connect(usePushButton, &QPushButton::released, this, [this]() {
    filterPreset("Use", "OPEN");
  });
  leftLayout->addWidget(usePushButton);

  _layout->insertLayout(0, leftLayout);
}

RepeaterBookResultsWidget::~RepeaterBookResultsWidget()
{
}

void RepeaterBookResultsWidget::filterPreset(const std::string& columnName, const std::string& regexStr)
{
  const auto& nameColumnMap = getNameColumnMap();
  if (nameColumnMap.find(columnName) == nameColumnMap.end())
    return;

  int j = nameColumnMap.at(columnName);
  std::string text("^(1|4)\\d\\d\\.");
  filterColumn(j, regexStr);
}
