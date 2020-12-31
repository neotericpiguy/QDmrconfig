#include "WidgetTests.hpp"

WidgetTests::WidgetTests() :
    QMainWindow(),
    _tabWidget(new QTabWidget(this)),
    _layout(new QVBoxLayout)
//    _confFileWidget(new ConfFileWidget(nullptr, nullptr))
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

  initChirpCsvTests();
  initBsonDocWidget();
  initConfFileWidget();
}

WidgetTests::~WidgetTests()
{
}

bool WidgetTests::initBsonDocWidget()
{
  Mongo::BSONDoc temp(RepeaterBookTests::repeaterStr);
  TEST(temp.has("results"), ==, true);
  TEST(temp.get<int32_t>("count"), ==, 51);
  auto bsonDocs = temp.get<std::vector<Mongo::BSONDoc>>("results");
  bsonDocs.resize(temp.get<int32_t>("count"));

  _tabWidget->addTab(new BSONDocWidget(bsonDocs), __PRETTY_FUNCTION__);
  return true;
}

bool WidgetTests::initConfFileWidget()
{
  //  _tabWidget->addTab(_confFileWidget, "ConfFileWidget");
  //
  //  auto& confFile = _confFileWidget->getConfFile();
  //  confFile.loadFile("examples/btech-6x2.conf");
  //  _confFileWidget->updateTabs();

  //  auto nameBlockMap = confFile.getNameBlocks();
  //
  //  TEST_EXP(nameBlockMap.find("Analog") != nameBlockMap.end());
  //
  //  if (nameBlockMap.find("Analog") == nameBlockMap.end())
  //    return false;
  //
  //  Mongo::BSONDoc temp(BSONDocTests::repeaterStr);
  //  TEST(temp.has("results"), ==, true);
  //  TEST(temp.get<int32_t>("count"), ==, 51);
  //  auto tempDoc = temp.get<std::vector<Mongo::BSONDoc>>("results");
  //  tempDoc.resize(temp.get<int32_t>("count"));
  //
  //  auto& confBlock = *nameBlockMap.at("Analog");
  //  TEST(confBlock.getLines().size(), ==, 298);
  //  const std::map<std::string, std::string> repeaterMap = {
  //      {"Name", "Callsign"},
  //      {"Receive", "Frequency"},
  //      {"Transmit", "Input Freq"},  // need offset not freq
  //      {"TxTone", "PL"},
  //  };
  //
  //  const std::map<std::string, std::string> columnDefault = {
  //      {"Power", "High"},
  //      {"Scan", "-"},
  //      {"TOT", "-"},
  //      {"RO", "-"},
  //      {"Admit", "-"},
  //      {"Squelch", "Normal"},
  //      {"RxTone", "-"},
  //      {"TxTone", "-"},
  //      {"Width", "25"},
  //      {"#", "#"},
  //  };
  //  confBlock.appendRepeaterDoc(tempDoc, repeaterMap, columnDefault);
  //  TEST(confBlock.getLines().size(), ==, 349);

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

bool WidgetTests::initChirpCsvTests()
{
  auto confFileWidget = new ConfFileWidget(nullptr, nullptr);
  auto& confFile = confFileWidget->getConfFile();
  confFile.loadFile("examples/btech-6x2.conf");
  confFileWidget->updateTabs();

  ChirpCsv chirpCsv;
  TEST(chirpCsv.open("./examples/not_exist_File"), ==, false);
  TEST(chirpCsv.open("./examples/Baofeng_UV-5R_20200529.csv"), ==, true);
  TEST(chirpCsv.size(), ==, 124);

  auto tempDocs = chirpCsv.getAnalogFormat();
  TEST(tempDocs.size(), ==, 124);

  auto& confBlock = *(confFile.getNameBlocks()).at("Analog");
  confBlock.appendRepeaterDoc(tempDocs);

  _tabWidget->addTab(confFileWidget, __PRETTY_FUNCTION__);
  return true;
}
