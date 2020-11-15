//
// Codebase_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

TEST( Codebase, i_can_open_a_codebase )
{
    thread::Pool pool( "twit.test.i_can_open_a_git_repository", 1 );
    Codebase codebase( pool );
    EXPECT_FALSE( codebase.is_open() );

    codebase.open( "/Users/will.baker/coin/twit" );
    EXPECT_TRUE( codebase.is_open() );
    
    codebase.close();
    EXPECT_FALSE( codebase.is_open() );
}
