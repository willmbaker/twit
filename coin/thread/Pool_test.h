//
// Pool_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "Pool.h"


using namespace coin;


TEST( Pool, i_can_create_a_thread_pool_with_a_single_thread )
{
    std::mutex              mutex;
    std::condition_variable wait_for_pool;

    std::atomic<bool> did_run      = false;
    std::atomic<bool> did_complete = false;

// GIVEN - I have a pool of size 1
    thread::Pool pool( "test", 1 );
    EXPECT_EQ( pool.name(), "test" );
    EXPECT_EQ( pool.size(), 1 );

// WHEN - I execute two processes
    pool.run
    (
        "test-two-processes",
        [&]( auto& process ) 
        { 
            auto lock = std::unique_lock( mutex );
            did_run = true; 
            wait_for_pool.notify_one();
        },
        [&]()                
        {
            did_complete = true; 
        }
    );

    auto lock = std::unique_lock( mutex );
    auto wait = wait_for_pool.wait_for( lock, test::standard_timeout_, [&]{ return did_run.load(); } );

    ASSERT_EQ( wait, true );

    pool.stop_all_and_wait();
    pool.update(); // This defines the current thread as the "control" thread, and thus has executed the completion callbacks for the process.

// THEN - Each of the two processes runs on a different thread
    EXPECT_TRUE( did_run );
    EXPECT_TRUE( did_complete );
}


TEST( Pool, i_can_create_a_thread_pool_with_many_threads )
{
    std::mutex               thread_mutex;
    std::vector<std::string> threads;
    std::condition_variable  wait_for_pool;
    int                      completed_processes = 0;

// GIVEN - I have a pool of size 2
    thread::Pool pool( "test", 2 );
    EXPECT_EQ( pool.name(), "test" );
    EXPECT_EQ( pool.size(), 2 );

    auto process = [&]( auto& process )
    {
        auto lock = std::unique_lock( thread_mutex );
        threads.push_back( process.thread->name );
        wait_for_pool.notify_one();
    };

    auto complete = [&]
    {
        ++completed_processes; 
    };

// WHEN - I submit two processes and wait for them to finish
    pool.run( "process-1", process, complete );
    pool.run( "process-2", process, complete );

    auto lock       = std::unique_lock( thread_mutex );
    auto no_timeout = wait_for_pool.wait_for( lock, test::standard_timeout_, [&]{ return threads.size() >= 2; });
    EXPECT_EQ( no_timeout, true );
    pool.stop_all_and_wait();
    pool.update();

// THEN - Each of the two processes runs on a different thread
    EXPECT_EQ( threads.size(), 2 );
    EXPECT_EQ( completed_processes, 2 );
}
