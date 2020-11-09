//
// log.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <ostream>

#include "log.h"


using namespace coin;


log::LogPipe::LogPipe( std::ostream& stream, bool break_on_message )
: stream_mutex_( new std::mutex() ),
  stream_( &stream ),
  break_on_message_( break_on_message )
{
}


log::LogPipe::LogPipe( std::ostream& stream, std::shared_ptr<std::mutex> stream_mutex, bool break_on_message )
: stream_mutex_( stream_mutex ),
  stream_( &stream ),
  break_on_message_( break_on_message )
{
}


static std::ofstream null_stream;

static log::LogPipe information_ = log::LogPipe( std::cout );
static log::LogPipe warn_        = log::LogPipe( std::cerr );
static log::LogPipe error_       = log::LogPipe( std::cerr );
static log::LogPipe null_        = log::LogPipe( null_stream );

static std::shared_ptr<std::mutex>   unified_mutex_;
static std::unique_ptr<std::ostream> unified_file_;


void
log::open_on_disk( const char* path )
{
    if( !unified_mutex_ ) unified_mutex_ = std::make_shared<std::mutex>();

    std::string unified = std::string(path) + "/unified.log";
    auto lock = std::unique_lock<std::mutex>( *unified_mutex_ );

    unified_file_ = std::unique_ptr<std::ostream>( new std::ofstream(unified.c_str()) );
    information_  = log::LogPipe( *unified_file_, unified_mutex_ );
    warn_         = log::LogPipe( *unified_file_, unified_mutex_ );
    error_        = log::LogPipe( *unified_file_, unified_mutex_ );
}


void
log::close()
{
    if( unified_mutex_ )
    {
        auto mutex = unified_mutex_;
        auto lock  = std::unique_lock<std::mutex>( *mutex );

        unified_file_->flush();

        unified_mutex_.reset();
        unified_file_.reset();
    }
    else
    {
        std::cout.flush();
        std::cerr.flush();
    }
}


log::LogPipe&
log::null()
{
    return null_;
}


log::LogPipe&
log::information( const char* file, int line, const char* channel )
{
    return information_ << "\nI " << "[" << channel << "] " << file << ":" << line << " ";
}


log::LogPipe&
log::warn( const char* file, int line, const char* channel )
{
    return warn_ << "\n! " << "[" << channel << "] " << file << ":" << line << " ";
}


log::LogPipe&
log::error( const char* file, int line, const char* channel )
{
    return error_ << "\nX " << "[" << channel << "] " << file << ":" << line << " ";
}
