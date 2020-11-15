//
// string.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_STRING_H_INCLUDED
#define COIN_STRING_H_INCLUDED

#include <string>
#include <vector>


namespace coin
{
	
namespace string
{

std::string join( const std::vector<std::string>& strings, const char* delimeter );

void for_lines_in( const std::string& string, std::function<void(const std::string&)> visitor );

}

}

#endif //COIN_STRING_H_INCLUDED
