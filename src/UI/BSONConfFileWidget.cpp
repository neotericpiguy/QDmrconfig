#include "BSONConfFileWidget.hpp"

BSONConfFileWidget::BSONConfFileWidget(const std::string& filename, QWidget* parent) :
    QWidget(parent),
    _bsonConfFile(),
    _confDoc(_bsonConfFile.getConfDoc()),
    _confDocs(_bsonConfFile.getConfDocs()),
    _channelDocs(_bsonConfFile.getChannelDocs()),
    _filename(filename),
    _tabWidget(nullptr)
{
  auto layout = new QVBoxLayout;
  setLayout(layout);

  if (!_bsonConfFile.loadFile(_filename))
  {
    layout->addWidget(new QLineEdit(QString::fromStdString("Could not open" + _filename)));
    return;
  }

  _tabWidget = new QTabWidget(this);
  layout->addWidget(_tabWidget);
  updateTabs();
}

BSONConfFileWidget::~BSONConfFileWidget()
{
  //    _tabWidget->addTab(temp, confBlock.getHeader().c_str());
}

void BSONConfFileWidget::updateTabs()
{
  _tabWidget->clear();

  for (const auto& pair : _confDocs)
  {
    const auto& key = pair.first;
    const auto& doc = pair.second;
    if (doc.has("Map"))
      _tabWidget->addTab(new QLineEdit(key.c_str()), key.c_str());
  }
}
