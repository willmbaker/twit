//
// Pool.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_THREAD_POOL_H_INCLUDED
#define COIN_THREAD_POOL_H_INCLUDED

#include <vector>
#include <deque>
#include <memory>
#include <thread>

#include <coin/log.h>

#include "LocklessQueue.h"


namespace coin
{

namespace thread
{

class Pool
{
public:
    class Process;

    class Thread
    ///
    /// A single thread in the pool. This will remain dormant until there is a process
    /// in the queue that needs to be processed, at which point a single thread will
    /// be awoken and complete the process. Finally, if it has not yet been asked to
    /// exit it will look for more work to do. If there is none then it will go
    /// back to sleep, to be resumed once there is more work for it to complete.
    ///
    {
    public:
        using ThreadFunction = std::function<void(const Thread&)>;

    private:
        std::thread thread_;

    public:
        const std::string name;
        std::atomic<bool> should_exit = { false };

    public:
        Thread( const std::string& name, ThreadFunction processing ) 
        : thread_( std::thread([this,processing]{ processing(*this); }) ),
          name( name )
        {
            COIN_ASSERT( thread_.joinable() ) << "The thread `" << name << "` is not running immediately after being created";
        }

        void wait_for()
        {
            if( thread_.joinable() )
            {
                thread_.join();
            }
        }
    };

    using ProcessCallback    = std::function<void(Process&)>;
    using CompletionCallback = std::function<void()>;
    using UpdateCallback     = std::function<void()>;
    using Processes          = std::deque<std::shared_ptr<Process>>;
    using Threads            = std::vector<std::shared_ptr<Thread>>;

    class Process
    ///
    /// A process represents a certain amount of work (ideally segmented into discrete blocks)
    /// that will run upon a background thread. Once this process is complete, a completion
    /// callback will be in turn raised on whichever thread `Pool.update()` is invoked.
    ///
    {
    public:
        const std::string        name;
        const ProcessCallback    process;
        const CompletionCallback complete;
        const Thread*            thread;

    public:
        Process( const std::string& name, ProcessCallback process, CompletionCallback complete )
        : name( name ),
          process( process ),
          complete( complete )
        {
        }
    };

private:
    std::string             name_;
    mutable std::mutex      queue_mutex_;
    std::condition_variable queue_ready_;
    thread::LocklessQueue   worker_to_control_;
    Threads                 threads_;
    Processes               processes_;
    int                     size_ = 0;

public:
    Pool( const std::string& name, int size );
    ~Pool(); //@!- This will block and wait until all processes are complete, which may not be appropriate.

    const std::string& name() const     { return name_; }
    int                size() const     { return size_; }
    bool               is_empty() const;

    void run( const std::string& name, ProcessCallback process, CompletionCallback completion={} );
    void stop_all();
    void stop_all_and_wait( UpdateCallback update = {} ); //@!- This should have a timeout, of some description, to avoid it waiting forever.
    void update();
};

}

}

#endif //COIN_THREAD_POOL_H_INCLUDED
