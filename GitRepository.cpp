//
// GitRepository.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <sstream>
#include <regex>

#include <coin/log.h>
#include <coin/string.h>
#include <coin/thread/Pool.h>
#include <coin/Process.h>

#include "GitRepository.h"


using namespace twit;
using namespace coin;


GitRepository::GitRepository( thread::Pool& pool )
: pool_( pool )
{
}


bool
GitRepository::is_open() const
{
    return !path_.empty();
}


void
GitRepository::open( const char* path )
{
    if( this->is_open() )
    {
        this->close();
    }

    path_ = path;
}


void
GitRepository::close()
{
    path_.clear();
}


void
GitRepository::start()
{
    COIN_ASSERT( this->is_open() ) << "A repository will not be started until it is open";
}


void
GitRepository::stop()
{
}


void
GitRepository::status( std::function<void(const Status&)> callback )
{
    auto status = std::make_shared<Status>();

    pool_.run( "GitRepository.status", [status]( thread::Pool::Process& )
    {
        Process process( "git" );
        std::stringstream output;

        int error = process.execute( {"status", "--porcelain", "--branch"}, output );
        if( !error )
        {
            std::regex branch_expression( "([A-Za-z-]+)\\.\\.\\.([A-Za-z-/]+) \\[(ahead|behind) ([0-9]+)\\]" );

            string::for_lines_in( output.str(), [&]( const std::string& line )
            {
                if( line[0] == '#' && line[1] == '#' )
                {
                    std::cmatch match;
                    if( std::regex_match(line.substr(3).c_str(), match, branch_expression) )
                    {
                        status->branch                = match[1];
                        status->remote_branch         = match[2];
                        status->distance_from_remote  = std::atoi( match[4].str().c_str() );
                        status->distance_from_remote *= (match[3] == "ahead") ? +1 : -1;
                    }
                    else
                    {
                        COIN_WARN( "twit" ) << "Failed to parse the branch status line `" << line << "`";
                    }

                }
                else if( line[0] == ' ' && line[1] == 'M' )
                {
                    status->modified.push_back( line.substr(3) );
                }
            });
        }
        else
        {
            status->error = CouldNotRetrieveStatus;
        }
    }, 
    [callback,status]
    {
        callback( *status );
    });
}


void
GitRepository::fetch()
{
}


void
GitRepository::push()
{
}


void
GitRepository::pull()
{
}

std::ostream& 
twit::operator<<( std::ostream& stream, const GitRepository::Status& status )
{
    stream << "Status on `" << status.branch << " " << status.remote_branch << "` (" << status.distance_from_remote << ")\n";

    for( const auto& modified : status.modified )
    {
        stream << modified << '\n';
    }

    return stream;
}
