#include <iostream>
#include "Logger\StandardOutputLogger.h"

int main()
{
  Logger logger = std::make_shared<CStandardOutputLogger>();
  logger->Info("Start JustChess");

  std::cout << "Let's play chess!" << std::endl;

  logger->Info("End JustChess");
}