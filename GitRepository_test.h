//
// GitRepository_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "GitRepository.h"


using namespace twit;


TEST( GitRepository, i_can_open_a_git_repository )
{
    thread::Pool pool( "twit.test.i_can_open_a_git_repository", 1 );
    GitRepository repository( pool );
    EXPECT_FALSE( repository.is_open() );

    repository.open( "/Users/will.baker/coin/twit" );
    EXPECT_TRUE( repository.is_open() );
    
    repository.close();
    EXPECT_FALSE( repository.is_open() );
}


TEST( GitRepository, i_can_get_the_status_of_a_repository )
//@!- This test is never going to be stable... you'd need to be creating the repository, I think.
{
    thread::Pool pool( "twit.test.i_can_get_the_status_of_a_repository", 1 );
    GitRepository repository( pool );
    repository.open( "/Users/will.baker/coin/twit" );

    GitRepository::Status status;
    bool status_complete = false;

    repository.status( [&]( const GitRepository::Status& current )
    {
        status          = current;
        status_complete = true;
    });

    while( !status_complete ) //@!- This should have a timeout or it's going to hang the test.
    {
        pool.update();
    }

    EXPECT_EQ( status.error, GitRepository::Success );
    EXPECT_EQ( status.branch, "main" );
    EXPECT_EQ( status.remote_branch, "origin/main" );
    EXPECT_EQ( status.distance_from_remote, +5 );
}
