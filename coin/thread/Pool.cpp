//
// Pool.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <thread>

#include "Pool.h"


using namespace coin;
using namespace coin::thread;


Pool::Pool( const std::string& name, int size )
: name_( name ),
  size_( size )
{
    auto processing = [this]( const Thread& thread )
    {
        do
        {
            COIN_VERBOSE( "thread" ) << "`" << thread.name << "` is waiting for work...";

            std::unique_lock<std::mutex> lock( queue_mutex_ );
            queue_ready_.wait( lock, [this,&thread]{ return thread.should_exit || !processes_.empty(); } );

            COIN_VERBOSE( "thread" ) << "`" << thread.name << "`" << (processes_.empty() ? " has been signalled to exit" : " has been woken for work");

            if( !thread.should_exit )
            {
                std::shared_ptr<Process> process = processes_.front();
                processes_.pop_front();
                COIN_VERBOSE( "thread" ) << "`" << thread.name << "` has claimed the first process in the queue";

                process->thread = &thread;
                process->process( *process );
                process->thread = nullptr;

                COIN_VERBOSE( "thread" ) << "`" << thread.name << "` has completed the process";

                worker_to_control_.call_later( [process]
                {
                    process->complete();
                });
            }
            else if( processes_.empty() )
            {
                COIN_WARN( "thread" ) << "`" << thread.name << "` has been signalled to exit with processes still in the queue";
            }
        }
        while( !thread.should_exit );

        COIN_VERBOSE( "thread" ) << thread.name << " will exit shortly";
    };

    for( int i = 0; i < size_; ++i )
    {
        std::string name = name_ + "-thread-" + std::to_string( i );
        threads_.push_back( std::make_shared<Thread>(name, processing) );
    }

    COIN_VERBOSE( "thread" ) << "`" << name_ << "` is running with " << threads_.size() << " threads";
}


Pool::~Pool()
{
    this->stop_all();          //@?- This will stop all worker threads that don't currently have work from picking up new work.
    this->stop_all_and_wait(); //@!- This will wait for work to be complete
}


bool
Pool::is_empty() const
{
    std::unique_lock<std::mutex> lock( queue_mutex_ );
    return processes_.empty();
}


void
Pool::run( const std::string& name, ProcessCallback process, CompletionCallback completion )
{
    std::unique_lock<std::mutex> lock( queue_mutex_ );
    processes_.push_back( std::make_shared<Process>(name, process, completion) );
    queue_ready_.notify_one();
}


void 
Pool::stop_all()
{
    COIN_VERBOSE( "thread" ) << "A thread pool with " << size_ << " threads is being shut down...";

    for( auto thread : threads_ )
    {
        thread->should_exit = true;
    }

    queue_ready_.notify_all();
}


void
Pool::stop_all_and_wait( UpdateCallback update )
{
//
// First, wait for everything in the queue to finish processing. This, perhaps
// more than anything, signals `Pool.stop_all_and_wait()` as a function really
// only suitable for those cases where you want to wait for a pool to completely
// drain, typically during tear-down or under test...
// 
//
    while( !this->is_empty() ) {} //@!- This signals, more than anything, the need for a timeout in this function. This could easily loop forever.

//
// ... then, allow the worker threads to exit their loops...
//
    this->stop_all();

//
// ... finally, wait for each thread to come to a complete stop.
//
    for( auto thread : threads_ )
    {
        COIN_VERBOSE( "thread" ) << "Waiting for `" << thread->name << "`";
        thread->wait_for();
        COIN_VERBOSE( "thread" ) << "`" << thread->name << "` complete";
    }

    if( update ) update();
}


void
Pool::update()
{
    worker_to_control_.flush();
}
