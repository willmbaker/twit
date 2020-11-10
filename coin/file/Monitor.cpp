//
// Monitor.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <CoreServices/CoreServices.h>

#include <coin/log.h>

#include "Monitor.h"


using namespace coin;
using namespace coin::file;


Monitor::Monitor()
{
    struct MacOSImplementation : public Monitor::Implementation
    {
        Monitor& monitor_;

        MacOSImplementation( Monitor& monitor )
        : monitor_( monitor )
        {
        }

        void if_file_changes( const char* path, ChangeCallback callback ) override
        {
        }
    };

    implementation_.reset( new MacOSImplementation(*this) );
}


static 
void 
fs_event_stream_callback
( 
    ConstFSEventStreamRef          stream, 
    void*                          context, 
    size_t                         event_count, 
    void*                          event_paths, 
    const FSEventStreamEventFlags* event_flags, 
    const FSEventStreamEventId*    event_identifiers 
)
{
}


void 
Monitor::if_file_changes( const char* path, ChangeCallback when_file_changes )
{
    FSEventStreamCallback    callback       = fs_event_stream_callback;
    FSEventStreamContext*    context        = nullptr;
    CFArrayRef               paths_to_watch = CFArrayCreate( kCFAllocatorDefault, nullptr, 0, nullptr ); //@!- This will be leaking, I'm pretty sure :/
    FSEventStreamEventId     since_when     = 0;
    CFTimeInterval           latency        = 1;
    FSEventStreamCreateFlags flags          = 0;

    FSEventStreamRef event_stream = FSEventStreamCreate
    ( 
        kCFAllocatorDefault, callback, context, 
        paths_to_watch, since_when, latency,
        flags
    );

    COIN_LOG( "monitor" ) << event_stream;
}
