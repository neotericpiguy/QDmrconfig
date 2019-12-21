#include "ConfFileWidget.hpp"

#include <QtWidgets/QtWidgets>

#include "ConfBlockWidget.hpp"

ConfFileWidget::ConfFileWidget() :
    QWidget(),
    _tabWidget(new QTabWidget)
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(_tabWidget.get());
  setLayout(layout);
}

ConfFileWidget::~ConfFileWidget()
{
}

ConfFile& ConfFileWidget::getConfFile()
{
  return _confFile;
}

void ConfFileWidget::updateTabs()
{
  for (auto& [index, confBlock] : _confFile.getConfBlock())
    _tabWidget->addTab(new ConfBlockWidget(confBlock), confBlock.getHeader().c_str());
}

