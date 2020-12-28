#include "MainWindow.hpp"

MainWindow::MainWindow(const std::function<void(const std::string&)>& radioUploadFile, const std::function<void(const std::string&)>& radioDownloadFile) :
    QMainWindow(),
    _bsonResults(),
    _fccSearchString(),
    _repeaterBookSearchString(),
    _confFile(radioUploadFile, radioDownloadFile),
    _confFileWidget(new ConfFileWidget(_confFile)),
    _bsonDocWidget(new BSONDocWidget(_bsonResults)),
    _networkManager(new QNetworkAccessManager(this)),
    _repeaterBookNetworkManager(new QNetworkAccessManager(this))
{
  setCentralWidget(_confFileWidget);

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
    loadFile(filename);
  });

  connect(uploadAct, &QAction::triggered, this, [this]() {
    _confFileWidget->getConfFile().uploadFile();
    statusBar()->showMessage(tr("Uploaded..."), 2000);
  });

  connect(downloadAct, &QAction::triggered, this, [this]() {
    auto filename = QFileDialog::getSaveFileName(this, tr("Destination"), "./", tr("Config Files (*.conf)"));
    _confFileWidget->getConfFile().downloadFile(filename.toStdString());
    statusBar()->showMessage("Downloaded...", 2000);
    loadFile(filename);
  });

  connect(saveAct, &QAction::triggered, this, [this]() {
    qDebug() << "Saving file";
    _confFileWidget->getConfFile().saveFile();
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
    _confFileWidget->nextTab(+1);
  });

  connect(backTabAct, &QAction::triggered, this, [this]() {
    _confFileWidget->nextTab(-1);
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

  _bsonResults = results.get<std::vector<Mongo::BSONDoc>>("Licenses.License");
  statusBar()->showMessage(tr("FCC Results"));
  _bsonDocWidget->update();
  takeCentralWidget();
  setCentralWidget(_bsonDocWidget);
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
  _bsonResults = results.get<std::vector<Mongo::BSONDoc>>("results");

  // Ensure dimensions match the docs.
  _bsonResults.resize(count);

  statusBar()->showMessage(tr("FCC Results: ") + QString::number(count));
  _bsonDocWidget->update();
  takeCentralWidget();
  setCentralWidget(_bsonDocWidget);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (_confFileWidget->getConfFile().isModified())
  {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
                                                               tr("Save changes?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
      _confFileWidget->getConfFile().saveFile();
    }
    else
    {
      event->ignore();
    }
  }

  // Prevents weird shutting down issues
  // It's almost like the destructor hop around different tabs until they are
  // all gone
  _confFileWidget->clear();
  event->accept();
}

void MainWindow::setDebug(bool state)
{
  _confFileWidget->setDebug(state);
}

void MainWindow::loadFile(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Application"),
                         tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(filename), file.errorString()));
    return;
  }

  _confFileWidget->getConfFile().loadFile(filename.toStdString());
  _confFileWidget->updateTabs();

  statusBar()->showMessage(tr("File loaded"), 2000);
  takeCentralWidget();
  setCentralWidget(_confFileWidget);
}
