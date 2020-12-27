#include "WidgetTests.hpp"

WidgetTests::WidgetTests() :
    QMainWindow(),
    _tabWidget(new QTabWidget(this)),
    _layout(new QVBoxLayout),
    bsonDocs(),
    bsonDocWidget(new BSONDocWidget(bsonDocs))
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
  Mongo::BSONDoc temp(BSONDocTests::repeaterStr);
  TEST(temp.has("results"), ==, true);
  TEST(temp.get<int32_t>("count"), ==, 51);
  bsonDocs = temp.get<std::vector<Mongo::BSONDoc>>("results");
  _tabWidget->addTab(bsonDocWidget, "BSONDocWidget");

  bsonDocWidget->update();
  return true;
}
