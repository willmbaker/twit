//
// Monitor_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "Monitor.h"


using namespace coin;


TEST( Monitor, i_can_monitor_for_file_changes )
{
	file::Monitor monitor;

	bool did_change = false;
	monitor.if_file_changes( "/tmp/some_file.text", [&]
	{
		did_change = true;
	});

	file::touch( "/tmp/some_file.text" );

	EXPECT_TRUE( did_change );
}
