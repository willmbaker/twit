//
// string.cpp
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "string.h"


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
