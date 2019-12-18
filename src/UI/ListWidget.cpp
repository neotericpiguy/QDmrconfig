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
    tabWidget->addTab(new QPlainTextEdit(confblock.getHeader().c_str()), confblock.getHeader().c_str());
    qDebug() << "Adding tab: \"" << confblock.getHeader().c_str()<<"\"";
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

ConfBlock::ConfBlock()
{
}

void ConfBlock::setLines(const std::vector<std::string>& lines)
{
  if(lines.size() > 0)
  {
    _header = lines[0];

    std::string from= "# Table of ";
    std::string to= "";

    size_t start_pos = _header.find(from);
    if(start_pos != std::string::npos)
      _header.replace(start_pos, from.length(), to);

    from = "# ";
    start_pos = _header.find(from);
    if(start_pos != std::string::npos)
      _header.replace(start_pos, from.length(), to);

    if(_header =="#")
      _header = "Info";
  }
}

std::string ConfBlock::getHeader() const
{
  return _header;
}
