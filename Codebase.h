//
// Codebase.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef TWIT_CODEBASE_H_INCLUDED
#define TWIT_CODEBASE_H_INCLUDED

namespace coin { namespace thread { class Pool; } } 

namespace twit
{

class Codebase
{
    coin::thread::Pool& pool_;

public:
    Codebase( coin::thread::Pool& pool );

    void open( const std::string_view& path );
};

}

#endif //TWIT_CODEBASE_H_INCLUDED
