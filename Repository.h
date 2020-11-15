//
// Repository.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef TWIT_REPOSITORY_H_INCLUDED
#define TWIT_REPOSITORY_H_INCLUDED

#include <string>


namespace coin { namespace thread { class Pool; } }


namespace twit
{

class Repository
{
public:
    enum Error
    {
        Success = 0,
        CouldNotRetrieveStatus
    };

    struct Status
    {
        std::string              branch;
        std::string              remote_branch;
        int                      distance_from_remote = 0;
        std::vector<std::string> modified;
        Error                    error = Success;
    };

private:
    coin::thread::Pool& pool_;
    std::string         path_;

public:
    Repository( coin::thread::Pool& pool );

    bool is_open() const;

    void open( const std::string_view& path );
    void close();
    void start();
    void stop();

    void status( std::function<void(const Status&)> callback );
    void fetch();
    void push();
    void pull();
};

std::ostream& operator<<( std::ostream& stream, const Repository::Status& status );

}

#endif //TWIT_REPOSITORY_H_INCLUDED
