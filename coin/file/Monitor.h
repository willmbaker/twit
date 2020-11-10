//
// Monitor.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_FILE_MONITOR_H_INCLUDED
#define COIN_FILE_MONITOR_H_INCLUDED

#include <memory>
#include <string>
#include <vector>


namespace coin
{
    
namespace file
{

class Monitor
{
public:
    using ChangeCallback = std::function<void()>;

private:
    struct Implementation
    {
        virtual ~Implementation() = default;
        virtual void if_file_changes( const char* path, ChangeCallback callback ) = 0;
    };

    std::unique_ptr<Implementation> implementation_;

public:
    Monitor();

    void if_file_changes( const char* path, ChangeCallback callback );
};

}

}

#endif //COIN_FILE_MONITOR_H_INCLUDED
