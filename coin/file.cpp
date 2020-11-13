//
// file.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <sys/stat.h>
#include <cstdio>
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
