//
// Process.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_PROCESS_H_INCLUDED
#define COIN_PROCESS_H_INCLUDED

#include <string>
#include <vector>
#include <memory>


namespace coin
{
    
class Process
{
public:
    struct Implementation
    {
    	virtual ~Implementation() = default;

        virtual void open( const std::vector<std::string>& arguments ) = 0;
    	virtual void read_all( std::ostream& output ) = 0;
    };

private:
    std::unique_ptr<Implementation> implementation_;
    std::string                     path_;

public:
    Process( const char* path );

    int execute( const std::vector<std::string>& arguments );
    int execute( const std::vector<std::string>& arguments, std::ostream& output );
};

}

#endif //COIN_PROCESS_H_INCLUDED
