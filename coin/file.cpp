//
// file.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <sys/stat.h>
#include <cstdio>

#include <coin/log.h>
#include <coin/Process.h>

#include "file.h"


using namespace coin;


bool
file::exists( const char* path )
{
    struct stat buffer;   
    return stat( path, &buffer ) == 0;
}


file::Error
file::remove( const char* path )
{
    int error = std::remove( path );

    switch( error )
    {
        case 0  : return Error::None;
        default : return Error::Unknown;
    }
}


void  
file::touch( const char* path )
{
    Process touch( "touch" );
    touch.execute( {path} );
}


int
file::time_modified( const char* path )
{
    struct stat file_status;

    if( stat(path, &file_status) == 0 )
    {
        time_t modified = file_status.st_mtime;
        return modified;
    }
    else
    {
        COIN_WARN( "file" ) << "Could not obtain the status of `" << path << "`";
        return -1;
    }
}
