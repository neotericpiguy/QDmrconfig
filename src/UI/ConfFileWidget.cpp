#include "ConfFileWidget.hpp"

ConfFileWidget::ConfFileWidget(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile, QWidget* parent) :
    QWidget(parent),
    _confFile(radioUploadFile, radioDownloadFile),
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

void ConfFileWidget::nextTab(int step)
{
  int tab = _tabWidget->currentIndex() + 2 * _tabWidget->count();
  tab = (tab + step) % _tabWidget->count();

  _tabWidget->setCurrentIndex(tab);
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

  for (auto& pair : _confFile.getConfBlocks())
  {
    auto& confBlock = pair.second;
    auto temp = new ConfBlockWidget(confBlock, this);
    temp->setDebug(_isDebug);

    _tabWidget->addTab(temp, confBlock.getHeader().c_str());
    _confBlockWidgets.push_back(temp);
  }
}

void ConfFileWidget::setDebug(bool state)
{
  _isDebug = state;
}
