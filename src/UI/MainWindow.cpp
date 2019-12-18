#include "MainWindow.hpp"

#include <QtWidgets/QtWidgets>

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

  QAction* closeAct = new QAction(tr("&Close"), this);
  closeAct->setShortcuts(QKeySequence::Close);
  closeAct->setStatusTip(tr("Close"));

  //  connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
  connect(closeAct, &QAction::triggered, this, [=]() {
    qDebug() << "close act";
    close();
  });

  fileMenu->addAction(newAct);
  fileMenu->addAction(closeAct);

  setUnifiedTitleAndToolBarOnMac(true);
  statusBar()->showMessage(tr("Ready"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
//  QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QDmrconfig",
//                                                             tr("Are you sure?\n"),
//                                                             QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
//                                                             QMessageBox::Yes);
//  if (resBtn != QMessageBox::Yes)
//  {
//    event->ignore();
//  }
//  else
//  {
//    event->accept();
//  }
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

//  QTextStream in(&file);
//#ifndef QT_NO_CURSOR
//  QGuiApplication::setOverrideCursor(Qt::WaitCursor);
//#endif
//  textEdit->setPlainText(in.readAll());
//#ifndef QT_NO_CURSOR
//  QGuiApplication::restoreOverrideCursor();
//#endif
//
//  setCurrentFile(filename);
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

