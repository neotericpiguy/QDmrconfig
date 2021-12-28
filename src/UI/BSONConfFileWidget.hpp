#ifndef BSONCONFFILEWIDGET
#define BSONCONFFILEWIDGET

#pragma GCC diagnostic ignored "-Weffc++"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QtWidgets>
#pragma GCC diagnostic pop

#include <fstream>
#include <memory>

#include "BSONConfFile.hpp"
#include "BSONDoc.hpp"

class BSONConfFileWidget : public QWidget
{
  Q_OBJECT

public:
  BSONConfFileWidget(const std::string& filename, QWidget* parent = 0);
  BSONConfFileWidget(const BSONConfFileWidget&) = delete;
  BSONConfFileWidget& operator=(const BSONConfFileWidget&) = delete;
  virtual ~BSONConfFileWidget();

  void updateTabs();

private:
  BSONConfFile _bsonConfFile;

  Mongo::BSONDoc& _confDoc;
  std::map<std::string, Mongo::BSONDoc>& _confDocs;
  std::vector<Mongo::BSONDoc>& _channelDocs;

  std::string _filename;
  QTabWidget* _tabWidget;
};

#endif
