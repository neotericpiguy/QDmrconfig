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
  std::ofstream outStream("temp.cfg");
  for (auto& [index, confBlock] : confBlocks)
  {
    //    tabWidget->addTab(new QPlainTextEdit(confBlock.getConfLines().c_str()), confBlock.getHeader().c_str());
    tabWidget->addTab(new ConfBlockWidget(confBlock), confBlock.getHeader().c_str());
    outStream << confBlock.getConfLines() << std::endl;
  }
}

bool ListWidget::isModified() const
{
  for (auto& [index, confBlock] : confBlocks)
    if (confBlock.isModified())
      return true;
  return false;
}

void ListWidget::loadFile(const std::string& filename)
{
  qDebug() << "Loading file: " << filename.c_str();
  std::map<int, std::vector<std::string>> blocks;

  int blockId = 0;
  std::ifstream inStream(filename);

  while (!inStream.eof())
  {
    std::string line;
    std::getline(inStream, line);

    if (!line.empty())
      blocks[blockId].push_back(line);

    if (line == "")
      blockId++;
  }
  qDebug() << "Blocks found: " << blocks.size();

  for (const auto& [key, value] : blocks)
    confBlocks[key].setLines(value);

  _filename = filename;

  updateTabs();
}

void ListWidget::saveFile()
{
  std::ofstream outStream(_filename);
  for (auto& [index, confBlock] : confBlocks)
  {
    outStream << confBlock.getConfLines() << std::endl;
    confBlock.setModified(false);
  }
}

