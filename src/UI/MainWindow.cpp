#include "MainWindow.hpp"

MainWindow::MainWindow(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile) :
    QMainWindow(),
    _fccSearchString(),
    _repeaterBookSearchString(),
    _networkManager(new QNetworkAccessManager(this)),
    _repeaterBookNetworkManager(new QNetworkAccessManager(this)),
    _tabWidget(new QTabWidget(this)),
    radioUploadFile(radioUploadFile),
    radioDownloadFile(radioDownloadFile)
{
  setCentralWidget(new QLineEdit(""));
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(_tabWidget);
  centralWidget()->setLayout(layout);
  _tabWidget->setTabsClosable(true);
  connect(_tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
    QWidget* tab = _tabWidget->widget(index);
    disconnect(tab, 0, 0, 0);
    tab->close();
    delete tab;
  });

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* openAct = new QAction(tr("&Open"), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Create a open file"));
  fileMenu->addAction(openAct);

  QAction* saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("SaveFile"));
  fileMenu->addAction(saveAct);

  QAction* searchAct = new QAction(tr("S&earch Callsign"), this);
  searchAct->setShortcut(QKeySequence(tr("Ctrl+E")));
  searchAct->setStatusTip(tr("Search Fcc"));
  fileMenu->addAction(searchAct);

  QAction* repeaterBookAct = new QAction(tr("Search Repeaterbook"), this);
  repeaterBookAct->setShortcut(QKeySequence(tr("Ctrl+r")));
  fileMenu->addAction(repeaterBookAct);

  QAction* uploadAct = new QAction(tr("&Upload"), this);
  uploadAct->setShortcut(QKeySequence(tr("Ctrl+U")));
  uploadAct->setStatusTip(tr("Upload file"));
  fileMenu->addAction(uploadAct);

  QAction* downloadAct = new QAction(tr("&Download"), this);
  downloadAct->setShortcut(QKeySequence(tr("Ctrl+D")));
  downloadAct->setStatusTip(tr("Download file"));
  fileMenu->addAction(downloadAct);

  QAction* closeAct = new QAction(tr("&Close"), this);
  closeAct->setShortcuts(QKeySequence::Close);
  closeAct->setStatusTip(tr("Close"));
  fileMenu->addAction(closeAct);

  QAction* changeTabAct = new QAction(tr("&Change Tab"), this);
  changeTabAct->setShortcut(QKeySequence(tr(("Ctrl+Tab"))));
  this->addAction(changeTabAct);

  QAction* backTabAct = new QAction(tr("&Change Tab"), this);
  backTabAct->setShortcut(QKeySequence(tr(("Ctrl+Shift+Tab"))));
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
    //    _confFileWidget->getConfFile().uploadFile();
    statusBar()->showMessage(tr("Uploaded..."), 2000);
  });

  connect(downloadAct, &QAction::triggered, this, [this]() {
    auto filename = QFileDialog::getSaveFileName(this, tr("Destination"), "./", tr("Config Files (*.conf)"));
    //    _confFileWidget->getConfFile().downloadFile(filename.toStdString());
    statusBar()->showMessage("Downloaded...", 2000);
    loadFile(filename.toStdString());
  });

  connect(saveAct, &QAction::triggered, this, [this]() {
    qDebug() << "Saving file";
    //    _confFileWidget->getConfFile().saveFile();
    statusBar()->showMessage(tr("Saved..."), 2000);
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

  connect(_networkManager, &QNetworkAccessManager::finished,
          this, &MainWindow::callsignSearchReady);

  connect(repeaterBookAct, &QAction::triggered, this, [this]() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search Repeater"),
                                         tr("Nearest City"), QLineEdit::Normal,
                                         _repeaterBookSearchString.c_str(), &ok);
    if (!ok)
      return;

    _repeaterBookSearchString = text.toStdString();

    QNetworkRequest request;
    QString url("https://www.repeaterbook.com/api/export.php?city=" + text);
    request.setUrl(url);

    _repeaterBookNetworkManager->get(request);
    statusBar()->showMessage("Searching Repeaterbook");
  });

  connect(_repeaterBookNetworkManager, &QNetworkAccessManager::finished,
          this, &MainWindow::repeaterBookSlotReadyRead);

  connect(closeAct, &QAction::triggered, this, [this]() {
    close();
  });

  connect(changeTabAct, &QAction::triggered, this, [this]() {
    //    _confFileWidget->nextTab(+1);
  });

  connect(backTabAct, &QAction::triggered, this, [this]() {
    //    _confFileWidget->nextTab(-1);
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
  _tabWidget->addTab(new BSONDocWidget(entries), QString("Repeater search: ") + QString(_repeaterBookSearchString.c_str()));
  _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
}

void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
  //  if (_confFileWidget->getConfFile().isModified())
  //  {
  //    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
  //                                                               tr("Save changes?\n"),
  //                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
  //                                                               QMessageBox::Yes);
  //    if (resBtn == QMessageBox::Yes)
  //    {
  //      _confFileWidget->getConfFile().saveFile();
  //    }
  //    else
  //    {
  //      event->ignore();
  //    }
  //  }
  //
  //  // Prevents weird shutting down issues
  //  // It's almost like the destructor hop around different tabs until they are
  //  // all gone
  //  _confFileWidget->clear();
  //  event->accept();
}

void MainWindow::setDebug(bool /*state*/)
{
  //  _confFileWidget->setDebug(state);
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
  confFileWidget->getConfFile().loadFile(filename);
  confFileWidget->updateTabs();

  statusBar()->showMessage(tr("File loaded"), 2000);
  _tabWidget->addTab(confFileWidget, filename.substr(filename.rfind("/") + 1, filename.length() - filename.rfind("/") - 1).c_str());
  _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
}
