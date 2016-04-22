#include "meerkat_logger.hpp"

/*
 * Initialize static members
 */
meerkat::mk_logger::SeverityLevel meerkat::mk_logger::_severityLevel = meerkat::mk_logger::Info;
clock_t meerkat::mk_logger::_startTime = clock();

meerkat::mk_logger::mk_logger()
{
    _tag = "";
    _severityLevel = Info;
}

meerkat::mk_logger::mk_logger( const std::string tag_ )
{
    _tag = tag_;
    _severityLevel = Info;
}

meerkat::mk_logger::mk_logger( const std::string tag_, const SeverityLevel severityLevel_ )
{
    _tag = tag_;
    _severityLevel = severityLevel_;
}

void meerkat::mk_logger::tag( std::string tag_ )
{
    _tag = tag_;
}

void meerkat::mk_logger::severity_level( SeverityLevel severityLevel_ )
{
    _severityLevel = severityLevel_;
}

void meerkat::mk_logger::i( const char *function_, const char *message_, ... ) const
{
    va_list arg;
    if( _severityLevel == Info ) {
        va_start(arg, message_);
        printf( "INFO %s  %s::%s: ", _get_time_str().c_str(), _tag.c_str(), function_ );
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}

void meerkat::mk_logger::w( const char *function_, const char *message_, ... ) const
{
    va_list arg;
    if( _severityLevel == Info ||_severityLevel == Warning ) {
        va_start(arg, message_);
        printf( "WARN %s  %s::%s: ", _get_time_str().c_str(), _tag.c_str(), function_ );
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}

void meerkat::mk_logger::e( const char *function_, const char *message_, ... ) const
{
    va_list arg;
    if( _severityLevel == Info || _severityLevel == Warning || _severityLevel == Error ) {
        va_start(arg, message_);
        printf( "ERRO %s  %s::%s: ", _get_time_str().c_str(), _tag.c_str(), function_ );
        vprintf(message_, arg);
        va_end(arg);
        printf("\n");
    }
}

std::string meerkat::mk_logger::_get_time_str() const
{
    char timestr[128] = {""};
    float fsec = (float)(clock() - _startTime)/CLOCKS_PER_SEC;
    int isec = (int)fsec;
    sprintf(timestr, "[%02d:%02d:%02d.%03d]",
            (int)((isec%86400)/3600),
            (int)((isec%3600)/60),
            (int)(isec%60),
            (int)((fsec-isec)*1000));
    return std::string(timestr);
}
