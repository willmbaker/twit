//
// Process_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "Process.h"


TEST( Process, i_can_execute_a_process )
{
	Process process( "python3" );
	int status = process.execute( {"--version"} );
	EXPECT_EQ( status, 0 );
}


TEST( Process, i_can_receive_data_from_a_process )
{
	Process process( "python3" );

	std::stringstream output;
	int status = process.execute( {"--version"}, output );

	EXPECT_EQ( status, 0 );
	EXPECT_EQ( output.str(), "Python 3.7.5\n" ); //@?- Why 3.7.5? Because that's what's on my computer :P
}


TEST( Process, i_can_send_data_to_a_process )
{
}
