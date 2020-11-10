//
// value_test.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#include "value.h"


using namespace coin;


TEST( value, the_type_of_a_value_may_be_inferred_from_a_string )
{
    EXPECT_TRUE( value::is_boolean("true") );
    EXPECT_FALSE( value::is_boolean("atruent3321") );
    EXPECT_TRUE( value::is_integer("-112321") );
    EXPECT_TRUE( value::is_integer("5") );
    EXPECT_FALSE( value::is_integer("77hhd889") );
    EXPECT_TRUE( value::is_number("0.0021") );
    EXPECT_FALSE( value::is_number("3342") );
}


TEST( value, i_can_create_an_undefined_value )
{
    coin::value value;

    ASSERT_NE( value.get_if<coin::value::undefined_t>(), nullptr );
    EXPECT_TRUE( value.undefined() );
    EXPECT_EQ( value.type(), "undefined" );
    EXPECT_FALSE( value.is<bool>() );
    EXPECT_FALSE( value.is<int>() );
    EXPECT_FALSE( value.is<double>() );
    EXPECT_FALSE( value.is<std::string>() );
}


TEST( value, i_can_create_a_boolean_value )
{
    coin::value value = true;

    ASSERT_NE( value.get_if<bool>(), nullptr );
    EXPECT_FALSE( value.undefined() );
    EXPECT_EQ( value.type(), "boolean" );
    EXPECT_EQ( value, true );
    EXPECT_NE( value, false );
    EXPECT_TRUE( value );
    EXPECT_FALSE( value.is<int>() );
    EXPECT_FALSE( value.is<double>() );
    EXPECT_FALSE( value.is<std::string>() );
}


TEST( value, i_can_create_a_integer_value )
{
    coin::value value = 32;

    ASSERT_NE( value.get_if<int>(), nullptr );
    EXPECT_FALSE( value.undefined() );
    EXPECT_EQ( value.type(), "integer" );
    EXPECT_EQ( value, 32 );
    EXPECT_NE( value, 1012 );
    EXPECT_FALSE( value.is<bool>() );
    EXPECT_TRUE( value.is<int>() );
    EXPECT_FALSE( value.is<double>() );
    EXPECT_FALSE( value.is<std::string>() );
}


TEST( value, i_can_create_a_number_value )
{
    coin::value value = 33.323;

    ASSERT_NE( value.get_if<double>(), nullptr );
    EXPECT_FALSE( value.undefined() );
    EXPECT_EQ( value.type(), "number" );
    EXPECT_FLOAT_EQ( *value.get_if<double>(), 33.323 ); //@!- I should be able to 
    EXPECT_NE( value, 1.232 );
    EXPECT_FALSE( value.is<bool>() );
    EXPECT_FALSE( value.is<int>() );
    EXPECT_TRUE( value.is<double>() );
    EXPECT_FALSE( value.is<std::string>() );
}


TEST( value, i_can_create_a_string_value )
{
    coin::value value = std::string("Hello, world.");

    ASSERT_NE( value.get_if<std::string>(), nullptr );
    EXPECT_FALSE( value.undefined() );
    EXPECT_EQ( value.type(), "string" );
    EXPECT_EQ( value, "Hello, world." );
    EXPECT_NE( value, "Oi, o mundo" );
    EXPECT_FALSE( value.is<bool>() );
    EXPECT_FALSE( value.is<int>() );
    EXPECT_FALSE( value.is<double>() );
    EXPECT_TRUE( value.is<std::string>() );
}


TEST( value, i_can_add_a_floating_point_number_to_a_value )
{
    coin::value value = 45.0;
    coin::value result = value + 45.0;

    EXPECT_EQ( result, 90.0 );
}
