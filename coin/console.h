//
// console.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_CONSOLE_H_INCLUDED
#define COIN_CONSOLE_H_INCLUDED

#include "value.h"


namespace coin
{
	
namespace console
{

using Arguments       = std::map<std::string,coin::value>;
using ArgumentVisitor = std::function<void(const std::string&,const coin::value&)>;

inline
Arguments
parse_arguments( int argc, char** argv )
{
	Arguments arguments;

    for( int a = 1; a < argc; ++a )
    {
        std::string argument = argv[a];
        auto is_assignment = argument.find( '=' );

        if( is_assignment != std::string::npos )
        {
            std::string name  = argument.substr( 0, is_assignment );
            std::string value = argument.substr( is_assignment+1 );

            arguments[name] = value;
        }
        else
        {
            arguments["action"] = argument;
        }
    }

    return arguments;
}

inline
void
with_arguments( const Arguments& arguments, ArgumentVisitor visitor )
{
    for( auto& argument : arguments )
    {
        visitor( argument.first, argument.second );
    }
}

}

}

#endif //COIN_CONSOLE_H_INCLUDED
