#include "MainWindow.hpp"

#include "QDmrconfigEditor.h"

MainWindow::MainWindow() :
    QMainWindow(),
    textEdit(new QPlainTextEdit("asdwfowiejf")),
    listWidget(new ListWidget)
{
  setCentralWidget(listWidget.get());

  connect(textEdit->document(), &QTextDocument::contentsChanged, this, [=]() {
    //    setWindowModified(textEdit->document()->isModified());
  });

  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

  QAction* newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  fileMenu->addAction(newAct);

  QAction* uploadAct = new QAction(tr("&Upload"), this);
  uploadAct->setShortcut(QKeySequence(tr("Ctrl+U")));
  uploadAct->setStatusTip(tr("Upload file"));
  fileMenu->addAction(uploadAct);

  QAction* closeAct = new QAction(tr("&Close"), this);
  closeAct->setShortcuts(QKeySequence::Close);
  closeAct->setStatusTip(tr("Close"));
  fileMenu->addAction(closeAct);

  //  connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
  connect(uploadAct, &QAction::triggered, this, [=]() {
    qDebug() << "upload act";
    radio_connect();
    radio_download();
    radio_print_version(stdout);
    radio_save_image("backup.img");
    radio_parse_config("device.conf");
    radio_verify_config();
    radio_upload(1);
    radio_disconnect();
  });
  connect(closeAct, &QAction::triggered, this, [=]() {
    qDebug() << "close act";
    close();
  });

  setUnifiedTitleAndToolBarOnMac(true);
  statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (listWidget->isModified())
  {
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
                                                               tr("Save changes?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
      listWidget->saveFile();
    }
    else
    {
      event->ignore();
    }
  }
  event->accept();
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

  listWidget->loadFile(filename.toStdString());

  statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::setCurrentFile(const QString& filename)
{
  //  curFile = filename;
  //  textEdit->document()->setModified(false);
  //  setWindowModified(false);
  //
  //  QString shownName = curFile;
  //  if (curFile.isEmpty())
  //    shownName = "untitled.txt";
  //  setWindowFilePath(shownName);
}

