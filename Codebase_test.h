//
// Codebase_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "Codebase.h"


TEST( Codebase, i_can_open_a_codebase )
{
    thread::Pool pool( "twit.test.i_can_open_a_codebase", 1 );
    Codebase codebase( pool );
    EXPECT_FALSE( codebase.is_open() );

    codebase.open( "/Users/will.baker/coin/twit" );
    EXPECT_TRUE( codebase.is_open() );
    
    codebase.close();
    EXPECT_FALSE( codebase.is_open() );
}


TEST( Codebase, i_can_build_a_codebase )
{
    thread::Pool pool( "twit.test.i_can_build_a_codebase", 1 );
    Codebase codebase( pool );
    codebase.open( "/Users/will.baker/coin/twit" );

    codebase.build();

    pool.stop_all_and_wait( [&]{ pool.update(); } );
    EXPECT_TRUE( codebase.is_assembled() );
    EXPECT_FALSE( codebase.has_build_error() );
}


TEST( Codebase, i_can_test_a_codebase )
{
    thread::Pool pool( "twit.test.i_can_test_a_codebase", 1 );
    Codebase codebase( pool );
    codebase.open( "/Users/will.baker/coin/twit" );

    codebase.test( "-Codebase.i_can_test_a_codebase" ); //@?- I can't run this test as part of itself because that would be an infinite loop.

    pool.stop_all_and_wait( [&]{ pool.update(); } );
    EXPECT_TRUE( codebase.is_clean() );
    EXPECT_FALSE( codebase.has_build_error() );
    EXPECT_FALSE( codebase.has_test_error() );
}
