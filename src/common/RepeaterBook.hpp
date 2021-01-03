#ifndef REPEATERBOOK_HPP
#define REPEATERBOOK_HPP

#include <iostream>
#include <string>
#include <vector>

#include "BSONDoc.hpp"
#include "ChannelSource.hpp"
#include "StringThings.hpp"

class RepeaterBook : public ChannelSource
{
public:
  RepeaterBook();
  virtual ~RepeaterBook();

  virtual bool fromStdString(const std::string& results) override;
  virtual std::vector<Mongo::BSONDoc> getAnalogFormat() const override;
  virtual bool append(const std::string& results) override;
};

#endif
