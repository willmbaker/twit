//
// binary.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_BINARY_H_INCLUDED
#define COIN_BINARY_H_INCLUDED

namespace coin
{
    
namespace binary
{

inline
uint32_t big_endian_to_little_endian( uint32_t bytes )
{
    uint32_t shifted = 
        ((bytes & 0x000000ff) << 24) | 
        ((bytes & 0x0000ff00) << 8)  |
        ((bytes & 0x00ff0000) >> 8)  |
        ((bytes & 0xff000000) >> 24)
    ;

    return shifted;
}


inline
uint32_t little_endian_to_big_endian( uint32_t bytes )
{
	uint32_t shifted =
		((bytes & 0x000000ff) << 24) |
		((bytes & 0x0000ff00) << 8)  |
		((bytes & 0x00ff0000) >> 8)  |
		((bytes & 0xff000000) >> 24)
	;

	return shifted;
}

}

}

#endif //COIN_BINARY_H_INCLUDED
