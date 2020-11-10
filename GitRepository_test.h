//
// GitRepository_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "GitRepository.h"


using namespace twit;


TEST( GitRepository, i_can_open_a_git_repository )
{
	GitRepository repository;
	EXPECT_FALSE( repository.is_open() );

	repository.open( "/Users/will.baker/coin/muse" );
	EXPECT_TRUE( repository.is_open() );
	
	repository.close();
	EXPECT_FALSE( repository.is_open() );
}
