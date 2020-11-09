//
// noncopyable.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_NONCOPYABLE_H_INCLUDED
#define COIN_NONCOPYABLE_H_INCLUDED

namespace coin
{

class noncopyable
{
public:
	noncopyable() = default;

private:
	noncopyable( const noncopyable& )            = delete;
	noncopyable& operator=( const noncopyable& ) = delete;
};

}

#endif //COIN_NONCOPYABLE_H_INCLUDED
