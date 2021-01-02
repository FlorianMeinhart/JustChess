#pragma once

/**
 *  @defgroup logging Logging
 */

/*!******************************************************************
* @interface ILogger
* 
* @ingroup logging
* 
* @brief Interface for logger.
* 
* @details There are three declared methods for logging:
* - @c Info()
* - @c Warning()
* - @c Error()
* 
* <tt>Logger</tt> is defined as <tt>std::shared_ptr<ILogger></tt>.
* 
* <b>Example:</b> Create a @c shared_ptr of type @c Logger
* by means of a derived class (e.g. @c CStandardOutputLogger).
* @code
* Logger logger = std::make_shared<CStandardOutputLogger>();
* logger->Info("Start JustChess");
* @endcode
* 
* @note Logging is realized via dependency injection.
* @see https://developer-blog.net/professionelles-loggen-unter-c/
* 
* @author Florian Meinhart
* @version 1.0
* @date 2020/12/26
********************************************************************/
class ILogger
{
public:
  virtual ~ILogger() = default;
  virtual void Info(std::string_view text, std::string_view file = "", int line = 0) = 0;
  virtual void Warning(std::string_view text, std::string_view file = "", int line = 0) = 0;
  virtual void Error(std::string_view text, std::string_view file = "", int line = 0) = 0;
};

using Logger = std::shared_ptr<ILogger>;
