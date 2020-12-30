#ifndef FIELDENTRYDIALOG_HPP
#define FIELDENTRYDIALOG_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QDialog>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QtWidgets>
#pragma GCC diagnostic pop

#include <iostream>
#include <memory>

class FieldEntryDialog : public QDialog
{
  Q_OBJECT

public:
  FieldEntryDialog(std::vector<std::string> _fields, std::vector<std::string>& _results, QWidget* parent = 0);
  FieldEntryDialog(const FieldEntryDialog&) = delete;
  FieldEntryDialog& operator=(const FieldEntryDialog&) = delete;
  virtual ~FieldEntryDialog();

public slots:
  virtual void done(int r);

private:
  std::vector<std::string> _fields;
  std::vector<std::string>& _results;
  std::vector<QLineEdit*> _resultsLineEdit;
};

#endif
