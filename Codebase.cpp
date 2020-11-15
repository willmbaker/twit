//
// Codebase.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <coin/thread/Pool.h>

#include "Codebase.h"


using namespace coin;
using namespace twit;


Codebase::Codebase( thread::Pool& pool )
: pool_( pool )
{
}


void
Codebase::open( const std::string_view& path )
{
}
