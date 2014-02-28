#ifndef BOOST_CONVERT_TEST_HPP
#define BOOST_CONVERT_TEST_HPP

#include "../include/api.hpp"
#include "../include/converter/lexical_cast.hpp"
#include "../include/converter/sstream.hpp"
#include "../include/converter/printf.hpp"
#include "../include/converter/strtol.hpp"
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iomanip>
#include <vector>
#include <list>
#include <iostream>
#include <stdio.h>

struct change
{
    typedef change this_type;

    enum value_type { no, up, dn };

    change(value_type v =no) : value_(v) {}

    friend std::istream& operator>>(std::istream& stream, this_type& dir)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") dir.value_ = up;
        else if (str == "dn") dir.value_ = dn;
        else if (str == "no") dir.value_ = no;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, this_type const& dir)
    {
        return stream << (dir.value_ == up ? "up" : dir.value_ == dn ? "dn" : "no");
    }

    value_type value() const { return value_; }

    private: value_type value_;
};

struct direction
{
    // Note: the class does NOT have the default constructor.

    enum value_type { up, dn };

    direction(value_type value) : value_(value) {}
    bool operator==(direction const& that) const { return value_ == that.value_; }

    friend std::istream& operator>>(std::istream& stream, direction& dir)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") dir.value_ = up;
        else if (str == "dn") dir.value_ = dn;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, direction const& dir)
    {
        return stream << (dir.value_ == up ? "up" : "dn");
    }

    private: value_type value_;
};

namespace boost
{
    template<> inline direction convert<direction>::create_storage() { return direction(direction::up); }
}

struct test
{
#if defined(_MSC_VER)
    static bool const    is_msc = true;
    static bool const    is_gcc = false;
    static int const num_cycles = 1000000;
#elif defined(__CYGWIN__) 
    static bool const    is_msc = false;
    static bool const    is_gcc = true;
    static int const num_cycles = 1000000;
#elif defined(__GNUC__)
    static bool const    is_msc = false;
    static bool const    is_gcc = true;
    static int const num_cycles = 10000000;
#else
#error "Add here."
#endif

    static void      sfinae();
    static void  algorithms();
    static void performance();

	template<typename Converter> static void type_to_string(Converter const&);
	template<typename Converter> static void string_to_type(Converter const&);
};

namespace cnv = boost::conversion;
namespace arg = boost::conversion::parameter;

#endif // BOOST_CONVERT_TEST_HPP
