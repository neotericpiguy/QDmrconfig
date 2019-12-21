#include "ConfBlockWidget.hpp"

#include <QtWidgets/QtWidgets>

ConfBlockWidget::ConfBlockWidget(ConfBlock& confBlock) :
    QWidget(),
    _confBlock(confBlock),
    _textView(new QPlainTextEdit()),
    _tableWidget(new QTableWidget())
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(_textView.get());
  layout->addWidget(_tableWidget.get());
  setLayout(layout);

  QFont f("unexistent");
  f.setStyleHint(QFont::Monospace);
  _textView->setFont(f);

  _textView->setReadOnly(true);
  _textView->setPlainText(_confBlock.getConfLines(false).c_str());

  if (_confBlock.isTable())
  {
    _tableWidget->setRowCount(_confBlock.getRowCount());
    _tableWidget->setColumnCount(_confBlock.getColumnCount());
    QStringList headers;

    for (const auto& c : _confBlock.getColumnNames())
      headers << c.c_str();

    _tableWidget->setHorizontalHeaderLabels(headers);
    _tableWidget->verticalHeader()->setVisible(false);

    const auto& lines = _confBlock.getLines();
    for (unsigned int i = 0; i < _confBlock.getRowCount(); i++)
    {
      const auto& line = lines[i];

      for (unsigned int j = 0; j < _confBlock.getColumnCount(); j++)
      {
        _tableWidget->setItem(i, j, new QTableWidgetItem(line[j].c_str()));
      }
    }
    //    connect(_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(&ConfBlockWidget::itemUpdate(QTableWidgetItem*)));
    //    connect(_tableWidget.get(), SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellSelected(int, int)));
    connect(_tableWidget.get(), SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(itemUpdate(QTableWidgetItem*)));
  }
}

ConfBlockWidget::~ConfBlockWidget()
{
}

void ConfBlockWidget::itemUpdate(QTableWidgetItem* item)
{
  int row = item->row();
  int column = item->column();
  auto& rows = _confBlock.getLines();

  rows[row][column] = item->text().toStdString();

  _confBlock.setModified(true);

  _textView->setPlainText(_confBlock.getConfLines(false).c_str());
}

void ConfBlockWidget::cellSelected(int nRow, int nCol)
{
  QMessageBox::information(this, "", "asdlfasjkfd");
}
