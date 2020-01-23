#include "MainWindow.hpp"

MainWindow::MainWindow() :
    QMainWindow(),
    _confFileWidget(new ConfFileWidget)
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

  connect(openAct, &QAction::triggered, this, [=]() {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open Config"), "./", tr("Config Files (*.conf)"));
    loadFile(filename);
  });

  connect(uploadAct, &QAction::triggered, this, [=]() {
    _confFileWidget->getConfFile().uploadFile();
    statusBar()->showMessage(tr("Uploaded..."), 2000);
  });

  connect(downloadAct, &QAction::triggered, this, [=]() {
    auto filename = QFileDialog::getSaveFileName(this, tr("Destination"), "./", tr("Config Files (*.conf)"));
    _confFileWidget->getConfFile().downloadFile(filename.toStdString());
    statusBar()->showMessage("Downloaded...", 2000);
    loadFile(filename);
  });

  connect(saveAct, &QAction::triggered, this, [=]() {
    qDebug() << "Saving file";
    _confFileWidget->getConfFile().saveFile();
    statusBar()->showMessage(tr("Saved..."), 2000);
  });

  connect(closeAct, &QAction::triggered, this, [=]() {
    close();
  });

  connect(changeTabAct, &QAction::triggered, this, [=]() {
    _confFileWidget->nextTab(+1);
  });

  connect(backTabAct, &QAction::triggered, this, [=]() {
    _confFileWidget->nextTab(-1);
  });

  QNetworkAccessManager* manager = new QNetworkAccessManager(this);

  QNetworkRequest request;
  request.setUrl(QUrl("http://www.arrl.org/advanced-call-sign-search"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  //  request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

  QUrlQuery params;
  params.addQueryItem("data[Search][terms]", "KJ7LEY");
  params.addQueryItem("data[PubaccEn][entity_name]", "");
  params.addQueryItem("data[PubaccEn][city]", "");
  params.addQueryItem("data[PubaccEn][state]", "");
  params.addQueryItem("data[PubaccEn][zip_code]", "");
  params.addQueryItem("data[PubaccEn][type]", "");

  connect(manager, &QNetworkAccessManager::finished,
          this, &MainWindow::slotReadyRead);

  manager->post(request, params.query().toUtf8());

  //  QNetworkReply* reply = manager->get(request);
  //  connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
  setUnifiedTitleAndToolBarOnMac(true);
  statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotReadyRead(QNetworkReply* reply)
{
  qDebug() << "Ready";
  QByteArray bts = reply->readAll();
  QString str(bts);
  qDebug() << str;
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
}

bool Parser::startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& /*att*/)
{
  qDebug() << "Start: " << qName;
  return true;
}

bool Parser::endElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& qName)
{
  qDebug() << "stop: " << qName;
  return true;
}

