//
// log.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_ASSERT_H_INCLUDED
#define COIN_ASSERT_H_INCLUDED

#include <stdio.h>
#include <thread>
#include <iostream>
#include <fstream>


namespace coin
{

namespace log
{

class LogPipe
{
    std::shared_ptr<std::mutex> stream_mutex_;
    std::ostream*               stream_;
    bool                        break_on_message_ = false;

public:
    LogPipe( std::ostream& stream, bool break_on_message=false );
    LogPipe( std::ostream& unified_stream, std::shared_ptr<std::mutex> stream_mutex, bool break_on_message=false );

    template <typename T> LogPipe& operator<<( T value )
    {
    //@!- Rather than using a mutex to marshal access to the stream between threads, what I need to
    //    be doing is queueing up the value in a lockless queue to be consumed on a worker thread
    //    responsible for flushing data to disk when able to.
        auto lock = std::unique_lock<std::mutex>( *stream_mutex_ );
        *stream_ << value; 

        if( this->break_on_message_ )
        {
            stream_->flush();
            __asm__( "int3" );
        }

        return *this;
    }
};

void open_on_disk( const char* path );
void close();

LogPipe& null();
LogPipe& information( const char* file, int line, const char* channel );
LogPipe& warn( const char* file, int line, const char* channel );
LogPipe& error( const char* file, int line, const char* channel );

}

}


//
// # Log channels
//

#if COIN_LOG_ENABLED

#define COIN_LOG( channel )     coin::log::information( __FILE__, __LINE__, channel )
#define COIN_WARN( channel )    coin::log::warn( __FILE__, __LINE__, channel )
#define COIN_ERROR( channel )   coin::log::error( __FILE__, __LINE__, channel )

#if COIN_LOG_VERBOSE
#define COIN_VERBOSE( channel ) coin::log::information( __FILE__, __LINE__, channel )
#else
#define COIN_VERBOSE( channel ) coin::log::null()
#endif


#else

#define COIN_LOG( channel )     coin::log::null()
#define COIN_WARN( channel )    coin::log::null()
#define COIN_ERROR( channel )   coin::log::null()
#define COIN_VERBOSE( channel ) coin::log::null()

#endif


//
// # Assertions
//

#if COIN_ASSERT_ENABLED

#define COIN_ASSERT( expression )                                                           \
    (!(expression) ? coin::log::error( __FILE__, __LINE__, "failure" ) : coin::log::null()) \

#else

#define COIN_ASSERT( expression ) coin::log::null()

#endif

#endif //COIN_ASSERT_H_INCLUDED
