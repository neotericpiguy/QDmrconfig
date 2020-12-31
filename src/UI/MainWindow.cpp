#include "MainWindow.hpp"

#include "BSONDoc.hpp"
#include "BSONDocWidget.hpp"
#include "ConfFileWidget.hpp"
#include "FieldEntryDialog.hpp"

MainWindow::MainWindow(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile) :
    QMainWindow(),
    _fccSearchString(),
    _repeaterBookSearchString(),
    _networkManager(new QNetworkAccessManager(this)),
    _repeaterBookNetworkManager(new QNetworkAccessManager(this)),
    _tabWidget(new QTabWidget(this)),
    radioUploadFile(radioUploadFile),
    radioDownloadFile(radioDownloadFile),
    _debug(false)
{
  setCentralWidget(new QLineEdit(""));
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(_tabWidget);
  centralWidget()->setLayout(layout);
  _tabWidget->setTabsClosable(true);

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  QAction* openAct = new QAction(tr("&Open"), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Create a open file"));
  fileMenu->addAction(openAct);

  QAction* saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("SaveFile"));
  fileMenu->addAction(saveAct);

  QAction* exportAct = new QAction(tr("&Import Search to ConfFile"), this);
  fileMenu->addAction(exportAct);

  QAction* importAct = new QAction(tr("I&mport Chirp Csv to ConfFile"), this);
  fileMenu->addAction(importAct);

  QAction* closeAct = new QAction(tr("&Close"), this);
  closeAct->setShortcuts(QKeySequence::Close);
  closeAct->setStatusTip(tr("Close"));
  fileMenu->addAction(closeAct);

  QMenu* searchMenu = menuBar()->addMenu(tr("&Search"));
  QAction* searchAct = new QAction(tr("S&earch Callsign"), this);
  searchAct->setShortcut(QKeySequence(tr("Ctrl+E")));
  searchAct->setStatusTip(tr("Search Fcc"));
  searchMenu->addAction(searchAct);

  QAction* repeaterBookAct = new QAction(tr("Search Repeaterbook"), this);
  repeaterBookAct->setShortcut(QKeySequence(tr("Ctrl+r")));
  searchMenu->addAction(repeaterBookAct);

  QMenu* radioMenu = menuBar()->addMenu(tr("&Radio"));
  QAction* uploadAct = new QAction(tr("&Upload"), this);
  uploadAct->setShortcut(QKeySequence(tr("Ctrl+U")));
  uploadAct->setStatusTip(tr("Upload file"));
  radioMenu->addAction(uploadAct);

  QAction* downloadAct = new QAction(tr("&Download"), this);
  downloadAct->setShortcut(QKeySequence(tr("Ctrl+D")));
  downloadAct->setStatusTip(tr("Download file"));
  radioMenu->addAction(downloadAct);

  QAction* changeTabAct = new QAction(tr("&Change Tab"), this);
  changeTabAct->setShortcut(QKeySequence(tr(("Ctrl+PgUp"))));
  this->addAction(changeTabAct);

  QAction* backTabAct = new QAction(tr("&Change Tab"), this);
  backTabAct->setShortcut(QKeySequence(tr(("Ctrl+PgDown"))));
  this->addAction(backTabAct);

  connect(openAct, &QAction::triggered, this, [this]() {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open Config"), "./", tr("Config Files (*.conf)"));
    if (filename.length() <= 0)
    {
      return;
    }
    loadFile(filename.toStdString());
  });

  connect(uploadAct, &QAction::triggered, this, [this]() {
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->currentWidget());
    if (confFileWidget)
    {
      confFileWidget->getConfFile().uploadFile();
      statusBar()->showMessage(tr("Uploaded..."), 2000);
    }
    else
    {
      QMessageBox::critical(this, "Failed to Upload", tr("Tab in focus is not a ConfFile"));
    }
  });

  connect(downloadAct, &QAction::triggered, this, [this, radioUploadFile, radioDownloadFile]() {
    auto filename = QFileDialog::getSaveFileName(this, tr("Destination"), "./", tr("Config Files (*.conf)"));
    auto confFileWidget = new ConfFileWidget(radioUploadFile, radioDownloadFile);
    confFileWidget->getConfFile().downloadFile(filename.toStdString());
    statusBar()->showMessage("Downloaded...", 2000);
    loadFile(filename.toStdString());
  });

  connect(saveAct, &QAction::triggered, this, [this]() {
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->currentWidget());
    if (confFileWidget)
    {
      confFileWidget->getConfFile().saveFile();
      statusBar()->showMessage(tr("Saved..."), 2000);
    }
    else
    {
      QMessageBox::critical(this, "Failed to Upload", tr("Tab in focus is not a ConfFile"));
    }
  });

  connect(searchAct, &QAction::triggered, this, [this]() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search Fcc callsign"),
                                         tr("call sign"), QLineEdit::Normal,
                                         _fccSearchString.c_str(), &ok);
    if (!ok)
      return;

    _fccSearchString = text.toStdString();
    QString url("https://data.fcc.gov/api/license-view/basicSearch/getLicenses?format=json&searchValue=");
    url += text;

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    _networkManager->get(request);
    statusBar()->showMessage("Searching fcc callsign: " + text);
  });
  connect(_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::callsignSearchReady);

  connect(repeaterBookAct, &QAction::triggered, this, [this]() {
    const std::vector<std::string> fields = {
        "callsign",
        "city",
        "landmark",
        "state",
        "country",
        "county",
        "frequency",
    };
    std::vector<std::string> results;

    auto dialog = new FieldEntryDialog(fields, results);
    dialog->setModal(true);
    dialog->exec();

    if (results.empty())
      return;

    // if all elements are ""
    if (std::all_of(results.begin(), results.end(), [](const std::string& i) {
          return i.empty();
        }))
      return;

    std::string urlStr("https://www.repeaterbook.com/api/export.php?");
    for (unsigned int i = 0; i < results.size(); ++i)
    {
      if (results[i] == "")
        continue;

      results[i] = fields[i] + "=" + results[i];
    }

    // Remove blank vector elements
    results.erase(std::remove(results.begin(), results.end(), ""), results.end());

    urlStr += StringThings::vecToStr(results, "&");
    QNetworkRequest request;
    request.setUrl(QString(urlStr.c_str()));

    _repeaterBookNetworkManager->get(request);
    statusBar()->showMessage("Searching Repeaterbook:" + QString(urlStr.c_str()));
  });
  connect(_repeaterBookNetworkManager, &QNetworkAccessManager::finished,
          this, &MainWindow::repeaterBookSlotReadyRead);

  connect(closeAct, &QAction::triggered, this, [this]() {
    QWidget* tab = _tabWidget->currentWidget();
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(tab);
    if (confFileWidget && confFileWidget->getConfFile().isModified())
    {
      QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
                                                                 "Save changes",
                                                                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                 QMessageBox::Yes);
      if (resBtn == QMessageBox::Yes)
      {
        confFileWidget->getConfFile().saveFile();
      }
    }
    disconnect(tab, 0, 0, 0);
    tab->close();
    delete tab;
  });

  connect(exportAct, &QAction::triggered, this, [this]() {
    auto bsonDocWidget = dynamic_cast<BSONDocWidget*>(_tabWidget->currentWidget());
    if (bsonDocWidget)
    {
      std::vector<Mongo::BSONDoc> results = bsonDocWidget->getVisibleDocs();

      for (int i = 0; i < _tabWidget->count(); i++)
      {
        auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->widget(i));
        if (!confFileWidget)
          continue;
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Export",
                                                                   "Export into Analog " + _tabWidget->tabText(i),
                                                                   QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes)
        {
          auto nameBlockMap = confFileWidget->getConfFile().getNameBlocks();
          if (nameBlockMap.find("Analog") == nameBlockMap.end())
          {
            QMessageBox::critical(this, "Failed to export", tr("Couldn't find Analog tab"));
            continue;
          }

          Mongo::BSONDoc tempDoc;
          tempDoc.append("count", (unsigned)results.size());
          tempDoc.append("results", results);
          std::cout << tempDoc.toString() << std::endl;

          RepeaterBook repeaterBook;
          repeaterBook.fromStdString(tempDoc.toString());

          nameBlockMap.at("Analog")->appendRepeaterDoc(repeaterBook.getAnalogFormat());
          confFileWidget->setTab("Analog");
          _tabWidget->setCurrentIndex(i);
        }
      }
    }
    else
    {
      QMessageBox::critical(this, "Failed to export", tr("Tab in focus is not a BSONDoc"));
    }
  });

  connect(importAct, &QAction::triggered, this, [this]() {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open Chirp Exported csv"), "./", tr("Csv (*.csv)"));

    if (filename.isEmpty())
      return;

    ChirpCsv chirpCsv;
    if (!chirpCsv.open(filename.toStdString()))
      return;

    for (int i = 0; i < _tabWidget->count(); i++)
    {
      auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->widget(i));
      if (!confFileWidget)
        continue;
      QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Import",
                                                                 "Import into Analog " + _tabWidget->tabText(i),
                                                                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                 QMessageBox::Yes);
      if (resBtn == QMessageBox::Yes)
      {
        auto nameBlockMap = confFileWidget->getConfFile().getNameBlocks();
        if (nameBlockMap.find("Analog") == nameBlockMap.end())
        {
          QMessageBox::critical(this, "Failed to export", tr("Couldn't find Analog tab"));
          continue;
        }

        nameBlockMap.at("Analog")->appendRepeaterDoc(chirpCsv.getAnalogFormat());
        confFileWidget->setTab("Analog");
        _tabWidget->setCurrentIndex(i);
      }
    }
  });

  connect(changeTabAct, &QAction::triggered, this, [this]() {
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->currentWidget());
    if (!confFileWidget)
      return;
    confFileWidget->nextTab(+1);
  });

  connect(backTabAct, &QAction::triggered, this, [this]() {
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->currentWidget());
    if (!confFileWidget)
      return;
    confFileWidget->nextTab(-1);
  });

  connect(_tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
    QWidget* tab = _tabWidget->widget(index);
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(tab);
    if (confFileWidget && confFileWidget->getConfFile().isModified())
    {
      QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
                                                                 "Save changes to " + _tabWidget->tabText(index),
                                                                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                 QMessageBox::Yes);
      if (resBtn == QMessageBox::Yes)
      {
        confFileWidget->getConfFile().saveFile();
      }
    }
    disconnect(tab, 0, 0, 0);
    tab->close();
    delete tab;
  });

  setUnifiedTitleAndToolBarOnMac(true);
  statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::callsignSearchReady(QNetworkReply* reply)
{
  QString replyStr(reply->readAll());
  reply->deleteLater();

  Mongo::BSONDoc results(replyStr.toStdString());

  if (results.has("Errors"))
  {
    QString url("https://data.fcc.gov/api/license-view/basicSearch/getLicenses?format=json&searchValue=");
    url += QString(_fccSearchString.c_str());

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    _networkManager->get(request);
    statusBar()->showMessage(tr("Retrying search..."));
    return;
  }

  auto entries = results.get<std::vector<Mongo::BSONDoc>>("Licenses.License");
  statusBar()->showMessage(tr("FCC Results"));
  _tabWidget->addTab(new BSONDocWidget(entries), QString("FCC search: ") + QString(_fccSearchString.c_str()));
  _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
}

void MainWindow::repeaterBookSlotReadyRead(QNetworkReply* reply)
{
  QString replyStr(reply->readAll());
  reply->deleteLater();
  std::string url = reply->url().toString().toStdString();
  url = url.substr(url.rfind("?") + 1, url.length() - url.rfind("?") - 1);

  Mongo::BSONDoc results(replyStr.toStdString());

  if (!results.has("results") || !results.has("count"))
  {
    statusBar()->showMessage(tr("Done messed up... Try again later"));
    return;
  }

  int32_t count = results.get<int32_t>("count");
  auto entries = results.get<std::vector<Mongo::BSONDoc>>("results");

  // Ensure dimensions match the docs.
  entries.resize(count);

  statusBar()->showMessage(tr("Repeater Results: ") + QString::number(count));
  _tabWidget->addTab(new BSONDocWidget(entries), QString("Repeater search: ") + QString(url.c_str()));
  _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  for (int i = 0; i < _tabWidget->count(); i++)
  {
    _tabWidget->setCurrentIndex(i);
    auto confFileWidget = dynamic_cast<ConfFileWidget*>(_tabWidget->currentWidget());
    if (!confFileWidget)
      continue;

    if (confFileWidget->getConfFile().isModified())
    {
      QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
                                                                 "Save changes to " + _tabWidget->tabText(i),
                                                                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                 QMessageBox::Yes);
      if (resBtn == QMessageBox::Yes)
      {
        confFileWidget->getConfFile().saveFile();
      }
      else
      {
        event->ignore();
      }
    }
  }

  event->accept();
}

void MainWindow::setDebug(bool state)
{
  _debug = state;
}

void MainWindow::loadFile(const std::string& filename)
{
  QString filenameQstr(filename.c_str());
  QFile file(filenameQstr);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Application"),
                         tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(filenameQstr), file.errorString()));
    return;
  }

  auto confFileWidget = new ConfFileWidget(radioUploadFile, radioDownloadFile);
  confFileWidget->setDebug(_debug);
  confFileWidget->getConfFile().loadFile(filename);
  confFileWidget->updateTabs();

  statusBar()->showMessage(tr("File loaded"), 2000);
  _tabWidget->addTab(confFileWidget, filename.substr(filename.rfind("/") + 1, filename.length() - filename.rfind("/") - 1).c_str());
  _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
}
