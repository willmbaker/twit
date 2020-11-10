//
// string_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "string.h"


TEST( string, i_can_join_an_array_of_strings )
{
	std::vector<std::string> array = { "and", "all", "watched", "over", "by", "machines", "of", "loving", "grace." };
	std::string joined_with_spaces = string::join( array, " " );
	EXPECT_EQ( joined_with_spaces, "and all watched over by machines of loving grace." );

	std::string joined_with_commas_and_spaces = string::join( array, ", " );
	EXPECT_EQ( joined_with_commas_and_spaces, "and, all, watched, over, by, machines, of, loving, grace." );
}
