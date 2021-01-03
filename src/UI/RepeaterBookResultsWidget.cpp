#include "RepeaterBookResultsWidget.hpp"

#include <regex>

RepeaterBookResultsWidget::RepeaterBookResultsWidget(const std::vector<Mongo::BSONDoc>& bsonDocs, QWidget* parent) :
    BSONDocWidget(bsonDocs, parent)
{
}

RepeaterBookResultsWidget::~RepeaterBookResultsWidget()
{
}

