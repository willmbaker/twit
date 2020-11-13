//
// Monitor_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <chrono>
#include <thread>

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

	const int then = file::time_modified( "/tmp/some_file.text" );
	file::touch( "/tmp/some_file.text" );
	const int now = file::time_modified( "/tmp/some_file.text" );
	EXPECT_TRUE( now >= then );

	std::this_thread::sleep_for( std::chrono::seconds(1) );

	EXPECT_TRUE( did_change );
}
