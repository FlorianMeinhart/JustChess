#pragma once

#include "Logger.h"
#include <iostream>

class CStandardOutputLogger : public ILogger
{
public:
  virtual void Info(std::string_view text) override
  {
    std::cout << "[INFO] " << text << std::endl;
  }
  virtual void Warning(std::string_view text) override
  {
    std::cout << "[WARNING] " << text << std::endl;
  }
  virtual void Error(std::string_view text) override
  {
    std::cout << "[ERROR] " << text << std::endl;
  }
};