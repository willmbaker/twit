//
// string.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "string.h"
#include "log.h"


using namespace coin;


std::string 
string::join( const std::vector<std::string>& strings, const char* delimeter )
{
    std::string output;

    for( int i = 0; i < strings.size(); ++i )
    {
        const std::string& string = strings[i];

        output += (i == 0 ? "" : delimeter) + string;
    }

    return output;
}


void
string::for_lines_in( const std::string& string, std::function<void(const std::string&)> visitor )
//@!- This should be using `string_view` in order to avoid taking a copy of the component parts.
{
    const char* start = string.data();
    const char* end   = start;

    while( *end != '\0' )
    {
        while( *end != '\0' && *end != '\n' )
        {
            ++end;
        }

        visitor( std::string(start, end) );

        start = end + 1;
        end   = start;
    }
}
