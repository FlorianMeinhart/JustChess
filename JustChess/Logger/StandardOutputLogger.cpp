#include <stdafx.h>

#include "StandardOutputLogger.h"

void CStandardOutputLogger::Info(std::string_view text, std::string_view file, int line)
{
  std::cout << "[INFO] " << text <<
    (file.empty() ? "" : "\n\tFILE: ") << (file.empty() ? "" : file) <<
    (line ? "\n\tLINE: " : "") << (line ? std::to_string(line) : "") << std::endl ;
}

void CStandardOutputLogger::Warning(std::string_view text, std::string_view file, int line)
{
  std::cout << "[WARNING] " << text <<
    (file.empty() ? "" : "\n\tFILE: ") << (file.empty() ? "" : file) <<
    (line ? "\n\tLINE: " : "") << (line ? std::to_string(line) : "") << std::endl;
}

void CStandardOutputLogger::Error(std::string_view text, std::string_view file, int line)
{
  std::cout << "[ERROR] " << text <<
    (file.empty() ? "" : "\n\tFILE: ") << (file.empty() ? "" : file) <<
    (line ? "\n\tLINE: " : "") << (line ? std::to_string(line) : "") << std::endl;
}

