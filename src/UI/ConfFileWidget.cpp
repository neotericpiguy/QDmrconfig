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
  if ((unsigned int)(_tabWidget->currentIndex()) < _confBlockWidgets.size())
  {
    _confBlockWidgets[_tabWidget->currentIndex()]->metaUpdate();
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
    auto temp = std::make_unique<ConfBlockWidget>(confBlock, _confFile);
    temp->setDebug(_isDebug);

    _tabWidget->addTab(temp.get(), confBlock.getHeader().c_str());
    _confBlockWidgets.push_back(std::move(temp));
  }
}

void ConfFileWidget::setDebug(bool state)
{
  _isDebug = state;
}
