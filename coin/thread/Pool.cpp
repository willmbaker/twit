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
    this->wait_to_finish();
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
Pool::wait_to_finish()
{
    this->stop_all();

    for( auto thread : threads_ )
    {
        COIN_VERBOSE( "thread" ) << "Waiting for `" << thread->name << "`";
        thread->wait_for();
        COIN_VERBOSE( "thread" ) << "`" << thread->name << "` complete";
    }
}


void
Pool::update()
{
    worker_to_control_.flush();
}
