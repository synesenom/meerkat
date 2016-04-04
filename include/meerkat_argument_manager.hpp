/* meerkat argument manager
 *
 * A simple class for managing argument lists for C++ applications.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_ARGUMENT_MANAGER_H
#define MEERKAT_ARGUMENT_MANAGER_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <string>
#include <vector>

namespace meerkat {

class mk_argument_manager
{
public:
    /** Enums */
    enum ArgumentType {Integer, Double, String};    // Argument value types.
    enum ArgumentAction {Value, Flag};              // Argument action.
    enum ArgumentStatus {Unset, Set};               // Argument status values.
    enum ArgumentPriority {Optional, Required};     // Argument priority values.

    /**
     * @brief add_argument_entry  Constructor, sets description, usage and adds help menu as
     *                            a default argument.
     * @description_              Description of the application.
     * @usage_                    Usage of the application.
     */
    mk_argument_manager( const char *description_, const char *usage_ );

    /**
     * @brief add_flag    Adds a flag argument.
     * @param name_       Argument reference name.
     * @param helpEntry_  Help entry.
     * @param keyLong_    Long key.
     * @param keyShort_   Short key.
     */
    void add_flag( const char *name_,
                   const char *helpEntry_,
                   const char *keyLong_,
                   const char *keyShort_ );

    /**
     * @brief add_int        Adds an integer argument.
     * @param name_          Argument reference name.
     * @param helpEntry_     Help entry.
     * @param defaultValue_  Default value.
     * @param keyLong_       Long key.
     * @param keyShort_      Short key.
     * @param priority_      Priority.
     */
    void add_int( const char *name_,
                  const char *helpEntry_,
                  const int defaultValue_,
                  const char *keyLong_,
                  const char *keyShort_,
                  ArgumentPriority priority_ );

    /**
     * @brief add_double     Adds a double argument.
     * @param name_          Argument reference name.
     * @param helpEntry_     Help entry.
     * @param defaultValue_  Default value.
     * @param keyLong_       Long key.
     * @param keyShort_      Short key.
     * @param priority_      Priority.
     */
    void add_double( const char *name_,
                     const char *helpEntry_,
                     const double defaultValue_,
                     const char *keyLong_,
                     const char *keyShort_,
                     ArgumentPriority priority_ );

    /**
     * @brief add_string     Adds a string argument.
     * @param name_          Argument reference name.
     * @param helpEntry_     Help entry.
     * @param defaultValue_  Default value.
     * @param keyLong_       Long key.
     * @param keyShort_      Short key.
     * @param priority_      Priority.
     */
    void add_string( const char *name_,
                     const char *helpEntry_,
                     const std::string defaultValue_,
                     const char *keyLong_,
                     const char *keyShort_,
                     ArgumentPriority priority_ );

    /**
     * @brief print_help  Prints help menu.
     */
    void print_help() const;

    /**
     * @brief read_arguments  Reads argument list using the argc/argv parameters.
     * @param argc_           Number of arguments.
     * @param argv_           Array of arguments.
     */
    void read_arguments( int argc_, char **argv_ );

    /**
     * @brief show_settings  Prints argument values.
     */
    void show_settings() const;

    /**
     * @brief get_int   Retrieves value of integer argument.
     * @param argName_  Argument reference name.
     * @return          Argument value.
     */
    int get_int( const char *argName_ );

    /**
     * @brief get_double  Retrieves value of double argument.
     * @param argName_    Argument reference name.
     * @return            Argument value.
     */
    double get_double( const char *argName_ );

    /**
     * @brief get_string  Retrieves value of string argument.
     * @param argName_    Argument reference name.
     * @return            Argument value.
     */
    std::string get_string( const char *argName_ );

    /**
     * @brief get_flag  Retrieves a flag argument.
     * @param argName_  Argument reference name.
     * @return          True if flag is set, false otherwise.
     */
    bool get_flag( const char *argName_ );

private:
    struct Argument {
        std::string name;
        std::string helpEntry;
        std::string keyLong;
        std::string keyShort;
        std::string value;
        ArgumentType type;
        ArgumentAction action;
        ArgumentStatus status;
        ArgumentPriority priority;

        Argument( std::string name_,
                  std::string helpEntry_,
                  std::string keyLong_,
                  std::string keyShort_,
                  std::string defaultValue_,
                  ArgumentType type_,
                  ArgumentAction action_,
                  ArgumentPriority priority_ )
        {
            name = name_;
            helpEntry = helpEntry_;
            keyLong = "--" + keyLong_;
            keyShort = "-" + keyShort_;
            value = defaultValue_;
            type = type_;
            action = action_;
            priority = priority_;
            status = Unset;
        }
    };

    std::string _description;                       // Short descirption of the application.
    std::string _usage;                             // Usage of the application.
    std::vector<Argument> _arguments;               // Available arguments for the application.

    /**
     * @brief add_argument_entry  Adds a new argument entry.
     * @param name_               Reference name.
     * @param helpEntry_          Help entry.
     * @param defaultValue_       Default value in a char array format.
     * @param keyLong_            Long key.
     * @param keyShort_           Short key.
     * @param type_               Argument type.
     * @param action_             Argument action.
     * @param priority_           Priority.
     */
    void _add_argument_entry( const char *name_,
                              const char *helpEntry_,
                              const char *defaultValue_,
                              const char *keyLong_,
                              const char *keyShort_,
                              ArgumentType type_,
                              ArgumentAction action_,
                              ArgumentPriority priority_ );
};
}

#endif // MEERKAT_ARGUMENT_MANAGER_H
