/* meerkat logger.
 *
 * A simple class for controllable application wide logging.
 *
 * Copyright (c) 2016, Enys Mones.
 */


#ifndef MEERKAT_LOGGER_HPP
#define MEERKAT_LOGGER_HPP

#include "stdlib.h"
#include "stdio.h"
#include <string>
#include <time.h>
#include <stdarg.h>

namespace meerkat {

class mk_logger
{
public:
    enum SeverityLevel { Info, Warning, Error };    // Severity levels.

    /**
     * @brief mk_log  Empty constructor, sets severity level to info.
     */
    mk_logger();

    /**
     * @brief mk_log  Constructor with tag.
     * @param tag_    Tag of the logger..
     */
    mk_logger( const std::string tag_ );

    /**
     * @brief mk_log          Complete constructor.
     * @param tag_            Tag of the logger.
     * @param severityLevel_  Severity level.
     */
    mk_logger( const std::string tag_, const SeverityLevel severityLevel_ );

    /**
     * @brief tag   Sets logger tag.
     * @param tag_  Tag.
     */
    void tag( std::string tag_ );


    /**
     * @brief severity_level  Sets severity level.
     * @param severityLevel_  Severity level.
     */
    static void severity_level( SeverityLevel severityLevel_ );


    /**
     * @brief e         Prints info message.
     * @param message_  Info message.
     */
    void i( const char *message_, ... ) const;


    /**
     * @brief e         Prints warning message.
     * @param message_  Warning message.
     */
    void w( const char *message_, ... ) const;


    /**
     * @brief e         Prints error message.
     * @param message_  Error message.
     */
    void e( const char *message_, ... ) const;

private:
    std::string _tag;                     // Additional tag.
    static SeverityLevel _severityLevel;  // Severity level.
    static clock_t _startTime;            // Start time of the app.

    /**
     * @brief _get_time_str  Retrieves time string.
     * @return               Current elapsed time in string format.
     */
    std::string _get_time_str() const;
};

}

#endif // MEERKAT_LOGGER_HPP
