#pragma once

#include "Logger.h"

/*!******************************************************************
* @class CStandardOutputLogger
* 
* @ingroup logging
*
* @brief Logger to console.
*
* @details <b>Writes output to console.</b>
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
  void Info(std::string_view text, std::string_view file, int line) override;
  /// Writes a warning text to console.
  void Warning(std::string_view text, std::string_view file, int line) override;
  /// Writes an error text to console.
  void Error(std::string_view text, std::string_view file, int line) override;
};