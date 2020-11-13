//
// Monitor.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>

#include <coin/log.h>
#include <coin/string.h>

#include "Monitor.h"


using namespace coin;
using namespace coin::file;


Monitor::Monitor()
{
    struct MacOSImplementation : public Monitor::Implementation
    {
        Monitor&         monitor_;
        FSEventStreamRef event_stream_ = nullptr;

        MacOSImplementation( Monitor& monitor )
        : monitor_( monitor )
        {
        }

        ~MacOSImplementation()
        {
            if( event_stream_ )
            {
                // CFRelease( event_stream_ ); //@!- This line is crashing so I disabled it :P
            }
        }

        void start() override
        {
            if( event_stream_ )
            {
                CFRunLoopRef run_loop = CFRunLoopGetCurrent();

                FSEventStreamScheduleWithRunLoop( event_stream_, run_loop, kCFRunLoopDefaultMode );
                bool stream_did_start = FSEventStreamStart( event_stream_ );
                COIN_LOG( "file" ) << "The stream is starting";
                if( !stream_did_start ) COIN_ERROR( "file" ) << "Failed to schedule event stream on the current run loop when monitoring `" << string::join(monitor_.paths_, " ") << "`";
            }
            else
            {
                COIN_ERROR( "file" ) << "The file monitor may not be started before the files to watch are specified";
            }
        }

        void stop() override
        {
            CFRunLoopRef run_loop = CFRunLoopGetCurrent();

            if( event_stream_ )
            {
                COIN_LOG( "file" ) << "The stream is stopping";
                FSEventStreamStop( event_stream_ );
                FSEventStreamUnscheduleFromRunLoop( event_stream_, run_loop, kCFRunLoopDefaultMode );
            }
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
            MacOSImplementation* implementation = static_cast<MacOSImplementation*>( context );
            implementation->monitor_.paths_did_change( event_paths, event_count );
        }

        void if_path_changes( const char* path, ChangeCallback callback ) override
        {
            CFStringRef paths[0];
            paths[0] = CFStringCreateWithCString( kCFAllocatorDefault, path, kCFStringEncodingUTF8 );

            FSEventStreamContext*    context        = nullptr;
            CFArrayRef               paths_to_watch = CFArrayCreate( kCFAllocatorDefault, (const void **)paths, 1, &kCFTypeArrayCallBacks );
            FSEventStreamEventId     since_when     = kFSEventStreamEventIdSinceNow;
            CFTimeInterval           latency        = 0; //@!- It probably isn't best to have it raise immediately, depending on how many paths I'm watching.
            FSEventStreamCreateFlags flags          = kFSEventStreamCreateFlagUseCFTypes;

            event_stream_ = FSEventStreamCreate
            ( 
                kCFAllocatorDefault, fs_event_stream_callback, context, 
                paths_to_watch, since_when, latency, flags
            );

            CFRelease( paths_to_watch );
        }
    };

    implementation_.reset( new MacOSImplementation(*this) );
}


Monitor::~Monitor()
{
    this->stop();
}


void 
Monitor::start()
{
    COIN_ASSERT( implementation_ ) << "There is no implementation for the monitor attempting to watch `" << string::join(paths_, " ") << "`";
    implementation_->start();
}


void 
Monitor::stop()
{
    COIN_ASSERT( implementation_ ) << "There is no implementation for the monitor attempting to watch `" << string::join(paths_, " ") << "`";
    implementation_->stop();
}


void 
Monitor::if_path_changes( const char* path, ChangeCallback when_path_changes )
{
    COIN_ASSERT( "file" ) << "There is no implementation for the monitor attempting to watch `" << path << "`";
    implementation_->if_path_changes( path, when_path_changes );
    paths_.push_back( path );
}


void
Monitor::paths_did_change( void* event_paths, int event_count )
{
    COIN_LOG( "file" ) << "There were " << event_count << " events";
}
