//
// Process.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include <stdio.h>

#include <coin/log.h>
#include <coin/file.h>
#include <coin/string.h>

#include "Process.h"


using namespace coin;


Process::Process( const std::string_view& path )
: path_( path )
{
}


int
Process::execute()
{
    return this->execute( {} );
}


int
Process::execute( std::ostream& output )
{
    return this->execute( {}, output );
}


int 
Process::execute( const std::vector<std::string>& arguments )
{
    std::ofstream null_output; //@!- I don't know, for a fact, that this is the right thing to do for a no-op output.
    return this->execute( arguments, null_output );
}


int 
Process::execute( const std::vector<std::string>& arguments, std::ostream& output )
{
    constexpr int buffer_size = 512;

    struct MacOSImplementation : public Process::Implementation
    {
        Process& process_;
        FILE*    pipe_ = nullptr;
        std::vector<char> buffer_;

        MacOSImplementation( Process& process )
        : process_( process ),
          buffer_( buffer_size )
        {
        }

        ~MacOSImplementation()
        {
            if( pipe_ )
            {
                this->close();
            }
        }

        void open( const std::vector<std::string>& arguments ) override
        {
            std::string command = process_.path_;

            command += " " + string::join( arguments, " " );

            COIN_VERBOSE( "process" ) << "Executing command `" << command << "`";

            FILE* pipe = popen( command.c_str(), "r" );
            if( pipe )
            {
                pipe_ = pipe;
            }
            else
            {
                COIN_ERROR( "process" ) << "The process `" << process_.path_ << "` failed to open with the arguments `" << string::join(arguments, " ");
            }
        }

        int close() override
        {
            return pclose( pipe_ ) >> 8; //@!- I guess I'm not sure what is in the LSB here...
        }

        int read_all( std::ostream& output ) override
        {
            COIN_ASSERT( pipe_ ) << "A process will not be read from if it has not been successfully opened";

            while( fgets(buffer_.data(), buffer_.size(), pipe_) )
            {
                output << buffer_.data();
            }

            return this->close();
        }
    };

    implementation_.reset( new MacOSImplementation(*this) );
    implementation_->open( arguments );
    return implementation_->read_all( output );
}
