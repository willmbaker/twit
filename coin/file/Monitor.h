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

        virtual void start() = 0;
        virtual void stop()  = 0;

        virtual void if_file_changes( const char* path, ChangeCallback callback ) = 0;
    };

    std::unique_ptr<Implementation> implementation_;
    std::vector<std::string>		paths_;

public:
    Monitor();

    void start();
    void stop();

    void if_file_changes( const char* path, ChangeCallback callback );

private:
	void files_did_change( void* event_paths, int event_count );	
};

}

}

#endif //COIN_FILE_MONITOR_H_INCLUDED
