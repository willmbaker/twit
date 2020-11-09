//
// math.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_MATH_H_INCLUDED
#define COIN_MATH_H_INCLUDED

#include <cmath>
#include <algorithm>


namespace coin
{
    
namespace math
{

constexpr double pi     = M_PI;
constexpr double two_pi = 2 * pi;

template <typename Type> 
Type
clamp( Type value, Type minimum, Type maximum )
{
	return std::max( minimum, std::min(value, maximum) );
}

}

}

#endif //COIN_MATH_H_INCLUDED
