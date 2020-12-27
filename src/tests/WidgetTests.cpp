#include "WidgetTests.hpp"

WidgetTests::WidgetTests() :
    QMainWindow(),
    _tabWidget(new QTabWidget(this)),
    _layout(new QVBoxLayout),
    _bsonDocs(),
    _bsonDocWidget(new BSONDocWidget(_bsonDocs))
{
  setCentralWidget(new QLineEdit("text"));
  _layout->addWidget(_tabWidget);
  centralWidget()->setLayout(_layout);

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* openAct = new QAction(tr("&Open"), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Create a open file"));
  fileMenu->addAction(openAct);

  initBsonDocWidget();
}

WidgetTests::~WidgetTests()
{
}

bool WidgetTests::initBsonDocWidget()
{
  _tabWidget->addTab(_bsonDocWidget, "BSONDocWidget");

  Mongo::BSONDoc temp(BSONDocTests::repeaterStr);
  TEST(temp.has("results"), ==, true);
  TEST(temp.get<int32_t>("count"), ==, 51);
  _bsonDocs = temp.get<std::vector<Mongo::BSONDoc>>("results");
  _bsonDocs.resize(temp.get<int32_t>("count"));

  _bsonDocWidget->update();
  return true;
}
