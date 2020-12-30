#include "FieldEntryDialog.hpp"

FieldEntryDialog::FieldEntryDialog(std::vector<std::string> fields, std::vector<std::string>& results, QWidget* parent) :
    QDialog(parent),
    _fields(fields),
    _results(results),
    _resultsLineEdit(std::vector<QLineEdit*>(_fields.size(), nullptr))
{
  auto vlayout = new QVBoxLayout;
  for (unsigned int i = 0; i < _fields.size(); ++i)
  {
    const auto& field = _fields[i];
    auto leftLayout = new QHBoxLayout;

    leftLayout->addWidget(new QLabel(field.c_str()));
    _resultsLineEdit[i] = new QLineEdit();
    leftLayout->addWidget(_resultsLineEdit.at(i));
    vlayout->addLayout(leftLayout);
  }

  setLayout(vlayout);
}

FieldEntryDialog::~FieldEntryDialog()
{
}

void FieldEntryDialog::done(int r)
{
  _results.resize(_resultsLineEdit.size());

  for (unsigned int i = 0; i < _results.size(); ++i)
    _results[i] = _resultsLineEdit[i]->text().toStdString();

  QDialog::done(r);
}
