#include "meerkat_file_manager.hpp"

meerkat::mk_file_manager::mk_file_manager()
{
    _pointer = NULL;
    _state = Empty;
    _fileName = "";
}

meerkat::mk_file_manager::~mk_file_manager()
{
    if( _state != Empty && _pointer != NULL )
        fclose( _pointer );
}

void meerkat::mk_file_manager::_error( const char *message_ ) const
{
    // Print message.
    if( _fileName == "" )
        printf( "mk_file_manager error: %s.\n", message_ );
    else
        printf( "mk_file_manager error: %s (%s).\n", message_, _fileName.c_str() );

    // Close and exit.
    if( _pointer != NULL )
        fclose( _pointer );
    exit( 0 );
}

bool meerkat::mk_file_manager::_open( const std::string fileName_, FileState mode_ )
{
    // Check if file is already opened.
    if( _pointer != NULL || _state != Empty )
    {
        printf("mk_file_manager warning: file is already open (%s).\n", fileName_.c_str());
        return false;
    }

    // Set file name.
    _fileName = fileName_;

    // Set state.
    _state = mode_;

    // Try to open depending on the state.
    _pointer = NULL;
    switch( _state )
    {
    case Read:
        _pointer = fopen( _fileName.c_str(), "r" );
        break;
    case Write:
        _pointer = fopen( _fileName.c_str(), "w" );
        break;
    default:
        return false;
    }

    // Return true if managed to open file.
    if( _pointer == NULL )
        return false;
    else
        return true;
}

bool meerkat::mk_file_manager::read( const std::string fileName_ )
{
    return _open(fileName_, Read);
}

bool meerkat::mk_file_manager::write( const std::string fileName_ )
{
    return _open(fileName_, Write);
}

void meerkat::mk_file_manager::close()
{
    if( _pointer != NULL )
        fclose( _pointer );
    _state = Empty;
    _fileName = "";
}

void meerkat::mk_file_manager::reset()
{
    if (_state == Read)
        fseek( _pointer, 0, SEEK_SET );
}

int meerkat::mk_file_manager::rows() const
{
    if (_state == Read)
    {
        char line[1024] = {""};
        int r = 0;
        fseek( _pointer, 0, SEEK_SET );
        while( !feof(_pointer) )
        {
            fgets( line, sizeof(line), _pointer );
            r++;
        }
        fseek( _pointer, 0, SEEK_SET );
        return r-1;
    }
    else
    {
        printf("mk_file_manager warning: file is not open for write.");
        return -1;
    }
}

int meerkat::mk_file_manager::cols() const
{
    if (_state == Read)
    {
        char line[1024] = {""};
        char *line_ptr = fgets( line, sizeof(line), _pointer );
        char col[1024] = {""};
        int c = 0, shift = 0;
        fseek( _pointer, 0, SEEK_SET );
        while( sscanf(line_ptr, "%s%n", col, &shift) == 1 )
        {
            c++;
            line_ptr += shift;
        }
        fseek( _pointer, 0, SEEK_SET );
        return c;
    }
    else
    {
        printf("mk_file_manager warning: file is not open for write.");
        return -1;
    }
}

bool meerkat::mk_file_manager::put( std::vector<double> &data_ )
{
    if( _state == Write )
    {
        int len = (int)data_.size();
        fprintf( _pointer, "%lg", data_[0] );
        for(int i=1; i<len; i++)
            fprintf( _pointer, " %lg", data_[i] );
        fprintf( _pointer, "\n" );
        return true;
    }
    else
    {
        printf( "mk_file_manager warning: file is not open for write.\n" );
        return false;
    }
}

bool meerkat::mk_file_manager::put( char *text_ )
{
    if( _state == Write)
    {
        fprintf( _pointer, "%s\n", text_ );
        return true;
    }
    else
    {
        printf( "mk_file_manager warning: file is not open for write.\n" );
        return false;
    }
}

std::vector<double> meerkat::mk_file_manager::get_data()
{
    if(_state == Read)
    {
        int numDataElement = 0;
        int shift = 0;
        double dataElement = 0.0;
        char *dataLine, line[1024] = {""};
        std::vector<double> data;
        dataLine = fgets( line, sizeof(line), _pointer );
        while( sscanf(dataLine, "%lf%n", &dataElement, &shift) == 1 )
        {
            data.push_back( dataElement );
            numDataElement++;
            dataLine += shift;
        }
        return data;
    }
    else
    {
        _error( "file is not readable" );
        return std::vector<double>();
    }
}

std::string meerkat::mk_file_manager::get_line()
{
    if(_state == Read)
    {
        char line[1024] = {""};
        fgets( line, 1023, _pointer );
        return std::string(line);
    }
    else
    {
        _error( "file is not readable" );
        return "";
    }
}
