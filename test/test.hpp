#ifndef BOOST_CONVERT_TEST_HPP
#define BOOST_CONVERT_TEST_HPP

#include "../example/example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/converter/lexical_cast.hpp>
#include <boost/convert/converter/sstream.hpp>
#include <boost/convert/converter/printf.hpp>
#include <boost/convert/converter/strtol.hpp>
#include <boost/convert/converter/spirit.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/detail/lightweight_test.hpp>
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

    friend std::istream& operator>>(std::istream& stream, this_type& chg)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") chg.value_ = up;
        else if (str == "dn") chg.value_ = dn;
        else if (str == "no") chg.value_ = no;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, this_type const& chg)
    {
        return stream << (chg.value_ == up ? "up" : chg.value_ == dn ? "dn" : "no");
    }

    value_type value() const { return value_; }

    private: value_type value_;
};

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

    static void           scratchpad ();
    static void               sfinae ();
    static void           algorithms ();
    static void          performance ();
    static void           encryption ();
    static void            callables ();
    static void      lcast_converter ();
    static void    sstream_converter ();
    static void       sstream_locale ();
    static void sstream_manipulators ();
    static void        int_to_string ();
    static void        string_to_int ();
    static void       string_to_bool ();
    static void            user_type ();
    static void        force_in_type ();

    template<typename Converter> static void type_to_string(Converter const&);
    template<typename Converter> static void string_to_type(Converter const&);
};

namespace cnv = boost::conversion;
namespace arg = boost::conversion::parameter;

#endif // BOOST_CONVERT_TEST_HPP
