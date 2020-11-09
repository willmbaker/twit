//
// value.h
// (c) 2020 - 2021 willmbaker. All rights reserved.
//

#ifndef COIN_VALUE_H_INCLUDED
#define COIN_VALUE_H_INCLUDED

#include <variant>
#include <string>
#include <regex>


namespace coin
{

class value
{
public:
    struct undefined_t {};
    using  Variant = std::variant<undefined_t,bool,int,double,std::string>;

private:
    Variant variant_;

public:
    value()                            : variant_(undefined_t()) {}
    value( bool boolean )              : variant_(boolean)       {}
    value( int number )                : variant_(number)        {}
    value( double number )             : variant_(number)        {}
    value( const std::string& string ) : variant_(string)        {}
    value( const char* string )        : variant_(string)        {}

    operator bool() const
    {
        if     ( this->is<bool>()   ) return *this->get_if<bool>();
        else if( this->is<int>()    ) return *this->get_if<int>();
        else if( this->is<double>() ) return *this->get_if<double>();
        else
        {
            return false;
        }
    }

    operator int() const
    {
        if     ( this->is<bool>()   ) return *this->get_if<bool>();
        else if( this->is<int>()    ) return *this->get_if<int>();
        else if( this->is<double>() ) return *this->get_if<double>();
        else
        {
            return 0;
        }
    }

    operator double() const
    {
        if     ( this->is<bool>()   ) return *this->get_if<bool>();
        else if( this->is<int>()    ) return *this->get_if<int>();
        else if( this->is<double>() ) return *this->get_if<double>();
        else
        {
            return 0.0;
        }
    }

    std::string
    type() const
    {
        if     ( this->is<bool>()        ) return "boolean";
        else if( this->is<int>()         ) return "integer";
        else if( this->is<double>()      ) return "number";
        else if( this->is<std::string>() ) return "string";
        else                               return "undefined";
    }

    bool
    undefined() const
    {
        return this->is<undefined_t>();
    }

    template <typename Type>
    bool 
    is() const
    {
        return this->get_if<Type>() != nullptr;
    }


    template <typename Type>
    Type*
    get_if()
    {
        return std::get_if<Type>( &variant_ );
    }


    template <typename Type>
    const Type*
    get_if() const
    {
        return std::get_if<Type>( &variant_ );
    }


    static
    bool 
    is_boolean( const char* string )
    {
        return std::regex_match( string, std::regex("(true|false)") );
    }


    static
    bool 
    is_integer( const char* string )
    {
        return std::regex_match( string, std::regex("(\\+|-)?[0-9]+") );
    }


    static
    bool 
    is_number( const char* string )
    {
        return std::regex_match( string, std::regex("(\\+|-)?[0-9]+\\.[0-9]+") );
    }


    template <typename Type>
    bool
    operator!=( Type value ) const
    {
        if( const Type* this_value = this->get_if<Type>() )
        {
            return *this_value != value;
        }
        else
        {
            return true;
        }
    }


    template <>
    bool
    operator!=( const char* string ) const
    {
        if( const std::string* this_value = this->get_if<std::string>() )
        {
            return *this_value != string;
        }
        else
        {
            return false;
        }        
    }


    template <typename Type>
    bool
    operator==( Type value ) const
    {
        return !(*this != value);
    }


    template <>
    bool
    operator==( const char* value ) const
    {
        return !(*this != value);   
    }
};


template <typename Type>
inline coin::value
operator+( const coin::value& value, Type number )
{
    if( const Type* is_type = value.get_if<Type>() )
    {
        return *is_type + number;
    }
    else
    {
        return value;
    }
}


inline std::ostream& 
operator<<( std::ostream& stream, const coin::value& value )
{
    if( const value::undefined_t* is_undefined = value.get_if<value::undefined_t>() )
    {
        stream << "undefined";
    }
    else if( const bool* is_boolean = value.get_if<bool>() )
    {
        stream << *is_boolean;
    }
    else if( const int* is_number = value.get_if<int>() )
    {
        stream << *is_number;
    }
    else if( const double* is_number = value.get_if<double>() )
    {
        stream << *is_number;
    }
    else if( const std::string* is_string = value.get_if<std::string>() )
    {
        stream << *is_string;
    }

    return stream;
}

}

#endif //COIN_VALUE_H_INCLUDED
