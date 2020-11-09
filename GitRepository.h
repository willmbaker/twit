//
// GitRepository.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef TWIT_GITREPOSITORY_H_INCLUDED
#define TWIT_GITREPOSITORY_H_INCLUDED

#include <string>


namespace twit
{

class GitRepository
{
	std::string path_;

public:
	bool is_open() const;

	void open( const char* path );
	void close();
	void start();
	void stop();
};

}

#endif //TWIT_GITREPOSITORY_H_INCLUDED
