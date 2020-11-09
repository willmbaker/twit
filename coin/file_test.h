//
// file_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "file.h"

class file_ : public ::testing::Test
{
protected:
    void TearDown() override
    {
        file::remove( "test.file.i_can_remove_a_file.text" );
    }
};


TEST_F( file_, i_can_test_if_a_file_exists )
{
	EXPECT_TRUE( file::exists("./muse") ); //@!- This is going to be very flaky :P
	EXPECT_FALSE( file::exists("389989499030929090") ); //@!- This could technically fail as well... should I have some kind of sandbox set up?
}


TEST_F( file_, i_can_remove_a_file )
{
	const char* path = "test.file.i_can_remove_a_file.text";

	ASSERT_FALSE( file::exists(path) );
	FILE* file = fopen( path, "w" );
	ASSERT_TRUE( file );
	fclose( file );

	ASSERT_TRUE( file::exists(path) );
	file::Error error = file::remove( path );
	EXPECT_FALSE( file::exists(path) );
	EXPECT_EQ( error, file::Error::None );
}
