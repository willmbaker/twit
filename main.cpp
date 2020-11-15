//
// main.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <iostream>

#include <coin/thread/Pool.h>

#include "Repository.h"


using namespace twit;
using namespace coin;


int main( int argc, char** argv )
{
	thread::Pool pool( "twit-main-pool", 1 );
	Repository repository( pool );

	repository.open( "/Users/will.baker/coin/muse" );
	repository.start();
	repository.stop();
}
