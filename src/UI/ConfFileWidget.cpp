#include "ConfFileWidget.hpp"

ConfFileWidget::ConfFileWidget() :
    QWidget(),
    _tabWidget(new QTabWidget),
    _layout(new QVBoxLayout)
{
  _layout->addWidget(_tabWidget.get());
  setLayout(_layout.get());
  connect(_tabWidget.get(), SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
}

ConfFileWidget::~ConfFileWidget()
{
}

void ConfFileWidget::tabSelected()
{
  if (_confFile.getConfBlocks()[_tabWidget->currentIndex()].getHeader() == "Scanlist")
  {
    if (_confFile.getNameBlocks().find("Analog") != _confFile.getNameBlocks().end())
    {
      ConfBlock& sourceBlock = *(_confFile.getNameBlocks()["Analog"]);
      ConfBlock& destBlock = *(_confFile.getNameBlocks()["Scanlist"]);

      _confFile.updateChannelList(sourceBlock, "Scan", destBlock, "Channels");

      _confBlockWidgets[_tabWidget->currentIndex()]->update();
    }
  }
}

ConfFile& ConfFileWidget::getConfFile()
{
  return _confFile;
}

void ConfFileWidget::updateTabs()
{
  _tabWidget->clear();
  _confBlockWidgets.clear();

  for (auto& [index, confBlock] : _confFile.getConfBlocks())
  {
    auto temp = std::make_unique<ConfBlockWidget>(confBlock);
    _tabWidget->addTab(temp.get(), confBlock.getHeader().c_str());
    _confBlockWidgets.push_back(std::move(temp));
  }
}

