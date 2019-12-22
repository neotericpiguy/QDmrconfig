#include "ConfFileWidget.hpp"

ConfFileWidget::ConfFileWidget() :
    QWidget(),
    _tabWidget(new QTabWidget),
    _layout(new QVBoxLayout)
{
  _layout->addWidget(_tabWidget.get());
  setLayout(_layout.get());
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
  _tabWidget->clear();
  _confBlockWidgets.clear();

  for (auto& [index, confBlock] : _confFile.getConfBlock())
  {
    auto temp = std::make_unique<ConfBlockWidget>(confBlock);
    _tabWidget->addTab(temp.get(), confBlock.getHeader().c_str());
    _confBlockWidgets.push_back(std::move(temp));
  }
}

