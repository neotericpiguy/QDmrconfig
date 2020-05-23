#include "MainWindow.hpp"

MainWindow::MainWindow() :
    QMainWindow(),
    _confFileWidget(new ConfFileWidget),
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

  QAction* repeaterBookAct = new QAction(tr("Download Repeaterbook"), this);
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

  connect(searchAct, &QAction::triggered, this, [=]() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search Fcc callsign"),
                                         tr("call sign"), QLineEdit::Normal,
                                         "", &ok);
    if (!ok)
      return;

    QNetworkRequest request;
    request.setUrl(QUrl("http://www.arrl.org/advanced-call-sign-search"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("data[Search][terms]", text);
    params.addQueryItem("data[PubaccEn][entity_name]", "");
    params.addQueryItem("data[PubaccEn][city]", "");
    params.addQueryItem("data[PubaccEn][state]", "");
    params.addQueryItem("data[PubaccEn][zip_code]", "");
    params.addQueryItem("data[PubaccEn][type]", "a");

    _networkManager->post(request, params.query().toUtf8());
    qDebug() << "Searching fcc: " << text;
  });

  connect(repeaterBookAct, &QAction::triggered, this, [=]() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Search Fcc callsign"),
                                         tr("call sign"), QLineEdit::Normal,
                                         "", &ok);
    if (!ok)
      return;

    QNetworkRequest request;
    //    request.setUrl(QUrl("https://www.repeaterbook.com/repeaters/location_search.php?state_id=04&loc=Pima&type=county"));
    request.setUrl(QUrl(text));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    _repeaterBookNetworkManager->post(request, params.query().toUtf8());

    connect(_repeaterBookNetworkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::repeaterBookSlotReadyRead);
    qDebug() << "Searching repeaterbook: ";
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

  connect(_networkManager, &QNetworkAccessManager::finished,
          this, &MainWindow::slotReadyRead);

  setUnifiedTitleAndToolBarOnMac(true);
  statusBar()->showMessage(tr("Ready"));

  //  QNetworkRequest request;
  //  request.setUrl(QUrl("https://www.repeaterbook.com/repeaters/location_search.php?state_id=04&loc=Pima&type=county"));
  //  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  //
  //  QUrlQuery params;
  //  _repeaterBookNetworkManager->post(request, params.query().toUtf8());
  //
  //  connect(_repeaterBookNetworkManager, &QNetworkAccessManager::finished,
  //          this, &MainWindow::repeaterBookSlotReadyRead);

  //  repeaterBookSlotReadyRead(NULL);
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotReadyRead(QNetworkReply* reply)
{
  QByteArray bts = reply->readAll();
  QString str(bts);

  //  std::ofstream temp("reply.html");
  //  temp << str.toStdString() << std::endl;
  //
  //  std::ifstream t("reply.html");
  //  std::string fileStr;
  //
  //  t.seekg(0, std::ios::end);
  //  fileStr.reserve(t.tellg());
  //  t.seekg(0, std::ios::beg);
  //
  //  fileStr.assign((std::istreambuf_iterator<char>(t)),
  //                 std::istreambuf_iterator<char>());
  //

  std::string fileStr = str.toStdString();
  auto spot = fileStr.find("list2");
  if (spot != std::string::npos)
  {
    fileStr = fileStr.substr(spot, fileStr.length() - spot);

    spot = fileStr.find("div");
    fileStr = fileStr.substr(0, spot);

    ConfBlock::replaceRegex(fileStr, "\\t", "");
    ConfBlock::replaceRegex(fileStr, "<[^>]*>", "");
    ConfBlock::replaceRegex(fileStr, ".*>.*", "");
    ConfBlock::replaceRegex(fileStr, ".*<.*", "");
    ConfBlock::replaceRegex(fileStr, "\\r\\n\\s*", "\n");
  }
  else
    fileStr = "Not Found!";

  qDebug() << fileStr.c_str();

  reply->deleteLater();
}

void MainWindow::repeaterBookSlotReadyRead(QNetworkReply* reply)
{
  //////////////////////////////////////////////////////////////////////////
  QByteArray bts = reply->readAll();
  QString str(bts);

  std::string fileStr = str.toStdString();

  //////////////////////////////////////////////////////////////////////////
  //  std::ifstream t("reply.html");
  //  std::string fileStr;
  //
  //  t.seekg(0, std::ios::end);
  //  fileStr.reserve(t.tellg());
  //  t.seekg(0, std::ios::beg);
  //
  //  fileStr.assign((std::istreambuf_iterator<char>(t)),
  //                 std::istreambuf_iterator<char>());
  //////////////////////////////////////////////////////////////////////////
  std::ofstream t("reply.html");

  t << fileStr << std::endl;

  std::cout << "parsing" << std::endl;
  std::cout << fileStr << std::endl;

  auto spot = fileStr.find("Tone ");
  if (spot != std::string::npos)
  {
    fileStr = fileStr.substr(spot, fileStr.length() - spot);
    spot = fileStr.rfind("administrator");
    fileStr = fileStr.substr(0, spot);

    ConfBlock::replaceRegex(fileStr, "<font[^>]*>", "");
    ConfBlock::replaceRegex(fileStr, "</font>", "");
    ConfBlock::replaceRegex(fileStr, ".*more details\">", "");
    ConfBlock::replaceRegex(fileStr, ".*nowrap>", "");
    ConfBlock::replaceRegex(fileStr, "<td>", "");
    ConfBlock::replaceRegex(fileStr, "</td>", "");
    ConfBlock::replaceRegex(fileStr, "<a>", "");
    ConfBlock::replaceRegex(fileStr, "</a>", "");
    ConfBlock::replaceRegex(fileStr, " MHz", "");
    ConfBlock::replaceRegex(fileStr, ".*<.*", "");
    ConfBlock::replaceRegex(fileStr, "\\r\\n\\s*", "\n");
    ConfBlock::replaceRegex(fileStr, "\\n+", "\n");
  }
  else
    fileStr = "Not Found!";

  //  qDebug() << fileStr.c_str();

  std::stringstream ss(fileStr);
  std::string temp;
  std::vector<std::vector<std::string>> entries;
  entries.push_back({});
  while (std::getline(ss, temp))
  {
    if (!temp.empty())
      entries.back().push_back(temp);
    if (temp.find("OPEN") != std::string::npos)
      entries.push_back({});
    if (temp.find("CLOSE") != std::string::npos)
      entries.back() = {};
  }

  entries.erase(std::remove_if(entries.begin(), entries.end(), [=](const auto& vec) {
                  float offset;
                  if (vec.size() < 2 || !ConfBlock::strTo(vec[1], offset))
                    return true;

                  offset = std::fabs(offset);
                  static std::vector<float> badOffsets = {0.1, 0.5, 1, 1.6, 25, 12};
                  if (std::find(badOffsets.begin(), badOffsets.end(), offset) != badOffsets.end())
                    return true;

                  if (std::find(vec.begin(), vec.end(), "DSTR") != vec.end())
                    return true;

                  if (vec.size() == 7 && vec[2].find("CC") != std::string::npos)
                    return true;

                  return false;
                }),
                entries.end());

  //Analog  Name             Receive   Transmit Power Scan TOT RO Admit  Squelch RxTone TxTone Width # AnalogOffset            Zone
  //    8   SMPLX_CALL       146.520   +0       Low   6    -   -  -      Normal  -      -      25    # Analog-Digital-Digital  1
  for (auto& i : entries)
  {
    std::vector<std::string> newEntry(16);
    newEntry[0] = "0";
    newEntry[1] = i[i.size() - 2] + "-" + i[0];  //Name
    newEntry[2] = i[0];                          //Frequency
    newEntry[3] = i[1];                          //Offset
    newEntry[4] = "Low";                         // Power
    newEntry[5] = "1";                           // Scan
    newEntry[6] = "-";                           // TOT
    newEntry[7] = "-";                           // RO
    newEntry[8] = "-";                           // Admit
    newEntry[9] = "Normal";                      // Squelch
    newEntry[10] = "-";                          // RxTone
    newEntry[11] = "-";                          // TxTone
    newEntry[12] = "25";                         // Width
    newEntry[13] = "#";                          // #
    newEntry[14] = "+1";                         // +1
    newEntry[15] = "1";                          // Zone

    newEntry[1].erase(newEntry[1].find_last_not_of('0') + 1, std::string::npos);  // remove trailing 0s

    if (i.size() == 7)
    {
      // if the PL has a rx and tx
      if (i[2].find("/") != std::string::npos)
      {
        std::stringstream ss(i[2]);
        char temp;
        ss >> newEntry[10] >> temp >> newEntry[11];
      }
      else if (i[2].find("CC") != std::string::npos)
      {
        newEntry = {};
      }
      else
      {
        newEntry[11] = i[2];
      }
    }

    i = newEntry;
  }

  for (const auto& i : entries)
  {
    std::string line = std::to_string(i.size()) + ":";
    for (const auto& j : i)
      line += j + "|";
    std::cout << line << std::endl;
  }

  const auto& confBlocks = _confFileWidget->getConfFile().getNameBlocks();
  if (confBlocks.find("Analog") != confBlocks.end())
  {
    auto& confBlock = *confBlocks.at("Analog");
    auto& lines = confBlock.getRows();
    for (const auto& i : entries)
      lines.push_back(i);
  }

  reply->deleteLater();
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

