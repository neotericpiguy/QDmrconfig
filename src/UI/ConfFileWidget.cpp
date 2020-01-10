#include "ConfFileWidget.hpp"

ConfFileWidget::ConfFileWidget(QWidget* parent) :
    QWidget(parent),
    _confFile(),
    _isDebug(false),
    _filename(),
    _confBlockWidgets(),
    _tabWidget(new QTabWidget(this)),
    _layout(new QVBoxLayout)
{
  _layout->addWidget(_tabWidget);
  setLayout(_layout);
  connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
}

ConfFileWidget::~ConfFileWidget()
{
}

void ConfFileWidget::tabSelected()
{
  if ((unsigned int)(_tabWidget->currentIndex()) < _confBlockWidgets.size())
    _confBlockWidgets[_tabWidget->currentIndex()]->metaUpdate();
}

ConfFile& ConfFileWidget::getConfFile()
{
  return _confFile;
}

void ConfFileWidget::clear()
{
  _tabWidget->clear();
  _confBlockWidgets.clear();
}

void ConfFileWidget::updateTabs()
{
  _confBlockWidgets.clear();
  _tabWidget->clear();

  for (auto& [index, confBlock] : _confFile.getConfBlocks())
  {
    auto temp = new ConfBlockWidget(confBlock, this);
    temp->setDebug(_isDebug);

    _tabWidget->addTab(temp, confBlock.getHeader().c_str());
    _confBlockWidgets.push_back(std::move(temp));
  }
}

void ConfFileWidget::setDebug(bool state)
{
  _isDebug = state;
}
