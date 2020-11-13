//
// file.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_FILE_H_INCLUDED
#define COIN_FILE_H_INCLUDED

namespace coin
{
	
namespace file
{

enum class Error
{
	None,
	DoesNotExist,
	Unknown
};


bool  exists( const char* path );
Error remove( const char* path );
void  touch( const char* path );

}

}

#endif //COIN_FILE_H_INCLUDED
