/* meerkat file manager
 *
 * A simple class for managing file input/output processes.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_FILE_MANAGER_HPP
#define MEERKAT_FILE_MANAGER_HPP

#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <string>

namespace meerkat {

class mk_file_manager
{
private:
    /** Enums */
    enum FileState {Empty, Write, Read};  // File pointer states.

    FILE *_pointer;                       // Pointer to the file.
    int _state;                           // State of the pointer (Empty, Read or Write).
    std::string _fileName;                // Name of the file.

    /**
     * @brief perror    Prints an error message to the standard I/O.
     * @param message_  Message to show.
     */
    void _error( const char *message_ ) const;

    /**
     * @brief _open      Opens a file for a given operation.
     * @param fileName_  File name.
     * @param mode_      Mode of open.
     * @return           True if file could be open in selected mode, false otherwise.
     */
    bool _open( const std::string fileName_, FileState mode_ );

public:
    /**
     * @brief mk_file_manager  Empty constructor, sets pointer to NULL.
     */
    mk_file_manager();

    /**
     * @brief ~mk_file_manager  Destructor. Closes file.
     */
    ~mk_file_manager();

    /**
     * @brief read       Opens a file for read.
     * @param fileName_  File name.
     * @return           True if file could be open for read, false otherwise.
     */
    bool read( const std::string fileName_ );

    /**
     * @brief write      Opens a file for write.
     * @param fileName_  File name.
     * @return           True if file could be open for write, false otherwise.
     */
    bool write( const std::string fileName_ );

    /**
     * @brief close  Closes a file.
     */
    void close();

    /**
     * @brief go_to_top  Returns to the beginning of the file.
     */
    void reset();

    /**
     * @brief rows  Returns the number of rows in file.
     * @return      Number of rows if file is readable, -1 otherwise.
     */
    int rows() const;

    /**
     * @brief cols  Returns the number of columns in file.
     * @return      Number of columns if file is readable, -1 otherwise.
     */
    int cols() const;

    /**
     * @brief put    Writes some columns from a vector in file.
     * @param data_  Vector to write in file.
     * @return       True if data vector could be written in file, false otherwise.
     */
    bool put( std::vector<double> &data_ );

    /**
     * @brief put    Writes a text line in file.
     * @param line_  Line to write in file.
     * @return       True if line could be written in file, false otherwise.
     */
    bool put( char *line_ );

    /**
     * @brief get_data  Retrieves the column at current line as a vector.
     * @return          Vector containing the column data.
     */
    std::vector<double> get_data();

    /**
     * @brief get_line  Retrieves the current line.
     * @return          Current line of the file.
     */
    std::string get_line();
};

}

#endif // MEERKAT_FILE_MANAGERPP
