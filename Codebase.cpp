//
// Codebase.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <coin/thread/Pool.h>
#include <coin/Process.h>

#include "Codebase.h"


using namespace coin;
using namespace twit;


Codebase::Codebase( thread::Pool& pool )
: pool_( pool )
{
}


bool
Codebase::is_open() const
{
    return !path_.empty();
}


bool
Codebase::has_build_error() const
{
    return status_ == Codebase::BuildError;
}


bool
Codebase::has_test_error() const
{
    return status_ == Codebase::TestError;
}


bool
Codebase::is_assembled() const
{
    return status_ == Codebase::Assembled;
}


bool
Codebase::is_clean() const
{
    return status_ == Codebase::Clean;
}


void
Codebase::open( const std::string_view& path )
{
    path_ = path;
}


void
Codebase::close()
{
    path_.clear();
}


void
Codebase::build()
{
    auto status = std::make_shared<int>( -1 );

    pool_.run( "Codebase.build", [this,status]( thread::Pool::Process& )
    {
        Process build( build_executable_ );
        *status = build.execute();
    },
    [this,status]
    {
        if( *status == 0 )
        {
            status_ = Assembled;
        }
        else
        {
            status_ = BuildError;
        }
    });
}


void
Codebase::test( const std::string_view& filter )
{
    auto status = std::make_shared<int>( -1 );
    auto filter_s = std::string( filter );

    pool_.run( "Codebase.test", [this,status,filter_s]( thread::Pool::Process& )
    {
        Process test( test_executable_ );
        *status = test.execute( {"--gtest_filter=" + filter_s} );
    },
    [this,status]
    {
        if( *status == 0 )
        {
            status_ = Clean;
        }
        else
        {
            status_ = TestError;
        }
    });
}
