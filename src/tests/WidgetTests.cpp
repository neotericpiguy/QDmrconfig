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

  repeaterBookExport();
  initChirpCsvTests();
  initBsonDocWidget();
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

bool WidgetTests::repeaterBookExport()
{
  auto confFileWidget = new ConfFileWidget(nullptr, nullptr);
  auto& confFile = confFileWidget->getConfFile();
  confFile.loadFile("examples/btech-6x2.conf");
  confFileWidget->updateTabs();

  RepeaterBook repeaterBook;
  TEST(repeaterBook.open("./examples/not_exist_File"), ==, false);
  TEST(repeaterBook.open("./examples/Baofeng_UV-5R_20200529.csv"), ==, false);
  TEST(repeaterBook.fromStdString(RepeaterBookTests::repeaterStr), ==, true);
  TEST(repeaterBook.size(), ==, 51);
  auto docs = repeaterBook.getAnalogFormat();
  TEST(docs.size(), ==, 51);

  // Have the necessary keys to perform a append
  std::vector<std::string> keys = {"Analog", "Name", "Receive", "Transmit", "Power", "Scan", "TOT", "RO", "Admit", "Squelch", "RxTone", "TxTone", "Width", "#"};
  std::sort(keys.begin(), keys.end());

  auto docKeys = docs[0].getKeys();
  std::sort(docKeys.begin(), docKeys.end());
  TEST(std::equal(keys.begin(), keys.end(), docKeys.begin()), ==, true);

  auto tempDocs = repeaterBook.getAnalogFormat();
  TEST(tempDocs.size(), ==, 51);

  auto& confBlock = *(confFile.getNameBlocks()).at("Analog");
  confBlock.appendRepeaterDoc(tempDocs);

  _tabWidget->addTab(confFileWidget, __PRETTY_FUNCTION__);
  return true;
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
