#include <iostream>

#include "StandardOutputLogger.h"

void CStandardOutputLogger::Info(std::string_view text)
{
  std::cout << "[INFO] " << text << std::endl;
}

void CStandardOutputLogger::Warning(std::string_view text)
{
  std::cout << "[WARNING] " << text << std::endl;
}

void CStandardOutputLogger::Error(std::string_view text)
{
  std::cout << "[ERROR] " << text << std::endl;
}

