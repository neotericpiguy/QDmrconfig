#include "ListWidget.hpp"

#include <QtWidgets/QtWidgets>

ListWidget::ListWidget() :
    QWidget(),
    tabWidget(new QTabWidget)
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(tabWidget.get());
  setLayout(layout);
}

ListWidget::~ListWidget()
{
}

void ListWidget::updateTabs()
{
  for (const auto& [index, confblock] : confBlocks)
  {
    tabWidget->addTab(new QPlainTextEdit(confblock.getLines().c_str()), confblock.getHeader().c_str());
//    std::cout  << confblock.getLines().c_str() << std::endl;
  }
}

void ListWidget::loadFile(const std::string& filename)
{
  qDebug() << "Loading file: " << filename.c_str();
  std::map<int,std::vector<std::string>> blocks;

  int blockId = 0;
  std::ifstream inStream(filename);

  while (!inStream.eof())
  {
    std::string line;
    std::getline(inStream, line);

    blocks[blockId].push_back(line);

    if(line == "")
      blockId++;

  }
  qDebug() << "Blocks found: " << blocks.size();

  for(const auto& [key, value] : blocks)
    confBlocks[key].setLines(value);

  updateTabs();
}

