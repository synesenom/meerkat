#include "meerkat_argument_manager.hpp"

meerkat::mk_argument_manager::mk_argument_manager( const char *description_, const char *usage_ )
{
    // Set description/usage.
    _description = std::string( description_ );
    _usage = std::string( usage_ );

    // Add help menu as a default argument.
    add_flag("help", "Prints help menu.", "help", "h" );
}

void meerkat::mk_argument_manager::add_flag( const char *name_,
                                             const char *helpEntry_,
                                             const char *keyLong_,
                                             const char *keyShort_ )
{
    _add_argument_entry( name_,
                         helpEntry_,
                         "off",
                         keyLong_,
                         keyShort_,
                         String,
                         Flag,
                         Optional );
}

void meerkat::mk_argument_manager::add_int( const char *name_,
                                            const char *helpEntry_,
                                            const int defaultValue_,
                                            const char *keyLong_,
                                            const char *keyShort_,
                                            ArgumentPriority priority_ )
{
    char chValue[1024];
    sprintf(chValue, "%i", defaultValue_);
    _add_argument_entry( name_,
                         helpEntry_,
                         chValue,
                         keyLong_,
                         keyShort_,
                         Integer,
                         Value,
                         priority_ );
}

void meerkat::mk_argument_manager::add_double( const char *name_,
                                               const char *helpEntry_,
                                               const double defaultValue_,
                                               const char *keyLong_,
                                               const char *keyShort_,
                                               ArgumentPriority priority_ )
{
    char chValue[1024];
    sprintf(chValue, "%lg", defaultValue_);
    _add_argument_entry( name_,
                         helpEntry_,
                         chValue,
                         keyLong_,
                         keyShort_,
                         Integer,
                         Value,
                         priority_ );
}

void meerkat::mk_argument_manager::add_string( const char *name_,
                                               const char *helpEntry_,
                                               const std::string defaultValue_,
                                               const char *keyLong_,
                                               const char *keyShort_,
                                               ArgumentPriority priority_ )
{
    _add_argument_entry( name_,
                         helpEntry_,
                         defaultValue_.c_str(),
                         keyLong_,
                         keyShort_,
                         Integer,
                         Value,
                         priority_ );
}

void meerkat::mk_argument_manager::print_help() const
{
    // Determine max keyword length.
    int keyLength = 0;
    int maxKeyLength = 0;
    for(int i=0; i<(int)_arguments.size(); i++)
    {
        keyLength = (int)_arguments[i].keyLong.length() + (int)_arguments[i].keyShort.length() + 5;
        if( keyLength > maxKeyLength )
            maxKeyLength = keyLength;
    }

    // Print help menu.
    printf( "\n" );
    printf( "%s\n", _description.c_str() );
    printf( "%s\n", _usage.c_str() );
    if( (int)_arguments.size() > 0 )
    {
        printf( "Options:\n" );
        for(int i=0; i<(int)_arguments.size(); i++)
        {
            keyLength = (int)_arguments[i].keyLong.length() + (int)_arguments[i].keyShort.length();
            printf( "  %s,%s", _arguments[i].keyLong.c_str(), _arguments[i].keyShort.c_str() );
            for(int j=0; j<maxKeyLength-keyLength; j++)
                printf( " " );
            printf( "%s\n", _arguments[i].helpEntry.c_str() );
        }
    }
    printf( "\n" );
    exit(0);
}

void meerkat::mk_argument_manager::read_arguments( int argc_, char **argv_ )
{
    if( argc_ == 1 )
    {
        print_help();
        exit(0);
    }

    // Set arguments.
    for(int i=0; i<argc_; i++)
    {
        for(int j=0; j<(int)_arguments.size(); j++)
        {
            if( std::string(argv_[i]) == _arguments[j].keyLong ||
                    std::string(argv_[i]) == _arguments[j].keyShort )
            {
                if( _arguments[j].name == std::string("help") )
                    print_help();

                if( _arguments[j].action == Value )
                    _arguments[j].value = std::string( argv_[i+1] );
                else
                    _arguments[j].value = std::string( "on" );
                _arguments[j].status = Set;
            }
        }
    }

    // Check required arguments.
    int req_not_given = 0;
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if( _arguments[i].priority == Required && _arguments[i].status == Unset )
        {
            printf( "mk_argument_manager error: a required argument is not set (%s).\n",
                    _arguments[i].name.c_str() );
            req_not_given++;
        }
    }

    if( req_not_given > 0 )
        exit(0);
}

void meerkat::mk_argument_manager::show_settings() const
{
    // Determine max name length.
    int nameLength = 0;
    int maxNameLength = 0;
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        nameLength = (int)_arguments[i].name.length() + 5;
        if( nameLength > maxNameLength )
            maxNameLength = nameLength;
    }

    // Print settings.
    printf( "\n" );
    printf( "Settings:\n---------\n" );
    if( (int)_arguments.size() > 0 )
    {
        for(int i=0; i<(int)_arguments.size(); i++)
        {
            if( _arguments[i].name != "help" )
            {
                nameLength = (int)_arguments[i].name.length();
                printf( "%s:", _arguments[i].name.c_str() );
                for(int j=0; j<maxNameLength-nameLength; j++)
                    printf( " " );
                printf( "%s\n", _arguments[i].value.c_str() );
            }
        }
    }
    printf( "\n" );
}

int meerkat::mk_argument_manager::get_int( const char *argName_ )
{
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if( _arguments[i].name == std::string(argName_) )
        {
            if( _arguments[i].action == Value && _arguments[i].type == Integer )
            {
                int value;
                sscanf(_arguments[i].value.c_str(), "%i", &value);
                return value;
            }
            else
            {
                printf( "get_int_argument error: argument is not an integer (%s).\n", argName_ );
                exit(0);
            }
        }
    }
    printf( "get_int_argument error: no such argument (%s).\n", argName_ );
    exit(0);
}

double meerkat::mk_argument_manager::get_double( const char *argName_ )
{
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if( _arguments[i].name == std::string(argName_) )
        {
            if( _arguments[i].action == Value && _arguments[i].type == Double )
            {
                double value;
                sscanf(_arguments[i].value.c_str(), "%lg", &value);
                return value;
            }
            else
            {
                printf( "get_int_argument error: argument is not a double (%s).\n", argName_ );
                exit(0);
            }
        }
    }
    printf( "get_int_argument error: no such argument (%s).\n", argName_ );
    exit(0);
}

std::string meerkat::mk_argument_manager::get_string( const char *argName_ )
{
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if( _arguments[i].name == std::string(argName_) )
        {
            if( _arguments[i].action == Value && _arguments[i].type == String )
                return _arguments[i].value;
            else
            {
                printf( "get_int_argument error: argument is not a string (%s).\n", argName_ );
                exit(0);
            }
        }
    }
    printf( "get_int_argument error: no such argument (%s).\n", argName_ );
    exit(0);
}

bool meerkat::mk_argument_manager::get_flag( const char *argName_ )
{
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if( _arguments[i].name == std::string(argName_) )
        {
            if( _arguments[i].action == Flag && _arguments[i].type == String )
                return _arguments[i].value == "on";
            else
            {
                printf( "get_int_argument error: argument is not a string (%s).\n", argName_ );
                exit(0);
            }
        }
    }
    printf( "get_int_argument error: no such argument (%s).\n", argName_ );
    exit(0);
}

void meerkat::mk_argument_manager::_add_argument_entry( const char *name_,
                                                        const char *helpEntry_,
                                                        const char *defaultValue_,
                                                        const char *keyLong_,
                                                        const char *keyShort_,
                                                        ArgumentType type_,
                                                        ArgumentAction action_,
                                                        ArgumentPriority priority_ )
{
    // Check if argument name/keys are already present.
    for (int i=0; i<(int)_arguments.size(); i++)
    {
        if (_arguments[i].name == std::string(name_))
        {
            printf("mk_argument_manager error: argument already exists (%s).\n", name_);
            exit(0);
        }
        if (_arguments[i].keyLong == std::string(keyLong_))
        {
            printf("mk_argument_manager error: key already exists (%s).\n", keyLong_);
            exit(0);
        }
        if (_arguments[i].keyShort == std::string(keyShort_))
        {
            printf("mk_argument_manager error: key already exists (%s).\n", keyShort_);
            exit(0);
        }
    }

    // If argument is new, add to argument list.
    _arguments.push_back(Argument(name_, helpEntry_, keyLong_, keyShort_,
                                  defaultValue_, type_, action_, priority_));
}

