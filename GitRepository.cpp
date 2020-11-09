//
// GitRepository.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <coin/log.h>

#include "GitRepository.h"


using namespace twit;


bool
GitRepository::is_open() const
{
	return !path_.empty();
}


void
GitRepository::open( const char* path )
{
	if( this->is_open() )
	{
		this->close();
	}

	path_ = path;
}


void
GitRepository::close()
{
	path_.clear();
}


void
GitRepository::start()
{
	COIN_ASSERT( this->is_open() ) << "A repository will not be started until it is open";
}


void
GitRepository::stop()
{
}
