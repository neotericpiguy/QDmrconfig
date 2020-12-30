#include "WidgetTests.hpp"

WidgetTests::WidgetTests() :
    QMainWindow(),
    _tabWidget(new QTabWidget(this)),
    _layout(new QVBoxLayout),
    _bsonDocs(),
    _bsonDocWidget(new BSONDocWidget(_bsonDocs)),
    _confFileWidget(new ConfFileWidget(nullptr, nullptr))
{
  setCentralWidget(new QLineEdit("text"));
  _layout->addWidget(_tabWidget);
  centralWidget()->setLayout(_layout);

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* fieldEntryDialogAction = new QAction(tr("&Open"), this);
  fieldEntryDialogAction->setShortcuts(QKeySequence::Open);
  fieldEntryDialogAction->setStatusTip(tr("Create a open file"));
  fileMenu->addAction(fieldEntryDialogAction);
  connect(fieldEntryDialogAction, &QAction::triggered, this, [this]() {
    initFieldEntryDialog();
  });

  initConfFileWidget();
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

bool WidgetTests::initConfFileWidget()
{
  _tabWidget->addTab(_confFileWidget, "ConfFileWidget");

  auto& confFile = _confFileWidget->getConfFile();
  confFile.loadFile("examples/btech-6x2.conf");
  _confFileWidget->updateTabs();

  auto nameBlockMap = confFile.getNameBlocks();

  TEST_EXP(nameBlockMap.find("Analog") != nameBlockMap.end());

  if (nameBlockMap.find("Analog") == nameBlockMap.end())
    return false;

  Mongo::BSONDoc temp(BSONDocTests::repeaterStr);
  TEST(temp.has("results"), ==, true);
  TEST(temp.get<int32_t>("count"), ==, 51);
  auto tempDoc = temp.get<std::vector<Mongo::BSONDoc>>("results");
  tempDoc.resize(temp.get<int32_t>("count"));

  auto& confBlock = *nameBlockMap.at("Analog");
  TEST(confBlock.getLines().size(), ==, 298);
  confBlock.appendRepeaterDoc(tempDoc);
  TEST(confBlock.getLines().size(), ==, 349);

  return true;
}

bool WidgetTests::initFieldEntryDialog()
{
  std::vector<std::string> fields = {"Last name", "first name"};
  std::vector<std::string> results;

  auto dialog = new FieldEntryDialog(fields, results);
  dialog->setModal(true);
  dialog->exec();

  for (const auto& r : results)
    std::cout << r << std::endl;
  return true;
}
