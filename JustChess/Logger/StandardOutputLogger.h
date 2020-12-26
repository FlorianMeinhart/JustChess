#pragma once

#include "Logger.h"
#include <iostream>

/*!******************************************************************
* @class CStandardOutputLogger
* 
* @ingroup logging
*
* @brief Logger to console.
*
* @details <b>Writes output to console.<b/>
* 
* Overrides three methods from @c ILogger:
* - @c Info()
* - @c Warning()
* - @c Error()
*
* @author Florian Meinhart
* @version 1.0
* @date 2020/12/26
********************************************************************/
class CStandardOutputLogger : public ILogger
{
public:
  /// Writes an info text to console.
  virtual void Info(std::string_view text) override
  {
    std::cout << "[INFO] " << text << std::endl;
  }
  /// Writes a warning text to console.
  virtual void Warning(std::string_view text) override
  {
    std::cout << "[WARNING] " << text << std::endl;
  }
  /// Writes an error text to console.
  virtual void Error(std::string_view text) override
  {
    std::cout << "[ERROR] " << text << std::endl;
  }
};