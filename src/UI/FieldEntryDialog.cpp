#include "FieldEntryDialog.hpp"

FieldEntryDialog::FieldEntryDialog(std::vector<std::string> fields, std::vector<std::string>& results, QWidget* parent) :
    QDialog(parent),
    _fields(fields),
    _results(results),
    _resultsLineEdit()
{
  //  auto cname = new QLineEdit;
  //  auto button = new QPushButton(tr("ok"));
  //  auto Warning = new QLabel(tr("Plase provide the name to new CoverLetter"));
  //  Warning->hide();

  auto vlayout = new QVBoxLayout;
  for (unsigned int i = 0; i < _fields.size(); ++i)
  {
    const auto& field = _fields[i];
    auto leftLayout = new QHBoxLayout;

    leftLayout->addWidget(new QLabel(field.c_str()));
    // _resultsLineEdit[i] = new QLineEdit();
    // leftLayout->addWidget(_resultsLineEdit[i]);
    leftLayout->addWidget(new QLineEdit());
    vlayout->addLayout(leftLayout);
  }

  //  vlayout->addWidget(Warning);
  setLayout(vlayout);
}

FieldEntryDialog::~FieldEntryDialog()
{
}

