//
// test.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <chrono>

#include <gtest/gtest.h>


namespace test
{

static const auto standard_timeout_ = std::chrono::milliseconds(300); //@!- It _kind_ of sucks that you're just supposed to know this is available in the `*_test.h` files.

}

#include "coin/value_test.h"
#include "coin/string_test.h"
#include "coin/file_test.h"
#include "coin/Process_test.h"
#include "coin/thread/LocklessQueue_test.h"
#include "coin/thread/Pool_test.h"

#include "GitRepository_test.h"


int main( int argc, char** argv )
{
    log::open_on_disk( ".log" );
    log::on_error( []
    {
    	FAIL();
    });

    ::testing::InitGoogleTest( &argc, argv );

    return RUN_ALL_TESTS();
}
