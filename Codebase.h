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
public:
    enum Status
    {
        Unknown,
        BuildError,
        TestError,
        Assembled,
        Clean
    };

private:
    coin::thread::Pool& pool_;
    std::string         path_;
    Status              status_ = Unknown;

    std::string build_executable_ = "/Users/will.baker/forge/bin/forge";
    std::string test_executable_  = "./test";

public:
    Codebase( coin::thread::Pool& pool );

    bool is_open() const;
    bool has_build_error() const;
    bool has_test_error() const;
    bool is_assembled() const;
    bool is_clean() const;

    void open( const std::string_view& path );
    void close();

    void build();
    void test( const std::string_view& filter = "" );
};

}

#endif //TWIT_CODEBASE_H_INCLUDED
