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


TEST( string, i_can_iterate_over_each_line_in_text )
{
    auto text = "This is\nmade up of\nseveral lines\n";

    std::vector<std::string> lines;
    string::for_lines_in( text, [&]( const std::string& line )
    {
        lines.push_back( line );
    });

    std::vector<std::string> expected = { "This is", "made up of", "several lines" };
    EXPECT_EQ( lines, expected );
}
