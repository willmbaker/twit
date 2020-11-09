//
// main.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <iostream>

#include "GitRepository.h"


using namespace twit;


int main( int argc, char** argv )
{
	GitRepository repository;

	repository.open( "/Users/will.baker/coin/muse" );
	repository.start();
	repository.stop();
}
