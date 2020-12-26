#pragma once

#include <memory>
#include <string_view>

/*!******************************************************************
* @interface ILogger
* 
* @brief Interface for Logger.
* 
* @details There are three declared methods:
* - @c Info()
* - @c Warning()
* - @c Error()
* 
* <tt>Logger</tt> is defined as <tt>std::shared_ptr<ILogger></tt>.
********************************************************************/
class ILogger
{
public:
  virtual ~ILogger() = default;
  virtual void Info(std::string_view text) = 0;
  virtual void Warning(std::string_view text) = 0;
  virtual void Error(std::string_view text) = 0;
};

using Logger = std::shared_ptr<ILogger>;