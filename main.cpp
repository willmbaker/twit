//
// main.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <iostream>

#include <coin/thread/Pool.h>

#include "Codebase.h"
#include "Repository.h"


using namespace twit;
using namespace coin;


int main( int argc, char** argv )
{
    std::string  path = "/Users/will.baker/coin/twit";
	thread::Pool pool( "twit-main-pool", 1 );

	Repository repository( pool );
	repository.open( path );
	repository.start();

    Codebase codebase( pool );
    codebase.open( path );
}
