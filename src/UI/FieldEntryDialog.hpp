#ifndef FIELDENTRYDIALOG_HPP
#define FIELDENTRYDIALOG_HPP

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QDialog>
#include <QtWidgets/QtWidgets>
#pragma GCC diagnostic pop

class FieldEntryDialog : public QDialog
{
  Q_OBJECT

public:
  FieldEntryDialog(const std::vector<std::string> _fields, std::vector<std::string>& _results, QWidget* parent = 0);
  FieldEntryDialog(const FieldEntryDialog&) = delete;
  FieldEntryDialog& operator=(const FieldEntryDialog&) = delete;
  virtual ~FieldEntryDialog();

public slots:
  virtual void accept();
  virtual void reject();

private:
  std::vector<std::string> _fields;
  std::vector<std::string>& _results;
  std::vector<QLineEdit*> _resultsLineEdit;
};

#endif
