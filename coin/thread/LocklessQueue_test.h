//
// LocklessQueue_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "LocklessQueue.h"


TEST( LocklessQueue, i_can_invoke_functions_in_the_queue )
{
	thread::LocklessQueue queue;

	std::vector<int> numbers;
	queue.call_later( [&]{ numbers.push_back(0); } );
	queue.call_later( [&]{ numbers.push_back(1); } );
	queue.call_later( [&]{ numbers.push_back(2); } );
	queue.call_later( [&]{ numbers.push_back(3); } );
	EXPECT_FALSE( queue.is_empty() );
	EXPECT_TRUE( numbers.empty() );

	queue.flush();

	std::vector<int> expected_numbers = { 0, 1, 2, 3 };
	EXPECT_EQ( numbers, expected_numbers );
	EXPECT_TRUE( queue.is_empty() );
}
