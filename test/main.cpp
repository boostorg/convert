// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "./str_to_int.hpp"
#include "./invalid.hpp"
#include <boost/convert/spirit.hpp>

using std::string;
using std::wstring;

template<typename Converter>
void
test::cnv::type_to_string(Converter const& cnv)
{
    BOOST_TEST("255" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::cnv::string_to_type(Converter const& cnv)
{
    BOOST_TEST( 255 == boost::convert<int>("255", cnv(arg::base = boost::cnv::base::dec)).value_or(999));
    BOOST_TEST( 999 == boost::convert<int>( "FF", cnv(arg::base = boost::cnv::base::dec)).value_or(999));
    BOOST_TEST( 255 == boost::convert<int>( "FF", cnv(arg::base = boost::cnv::base::hex)).value_or(999));
    BOOST_TEST( 173 == boost::convert<int>("255", cnv(arg::base = boost::cnv::base::oct)).value_or(999));
    BOOST_TEST( 999 != boost::convert<double>("1.23", cnv).value_or(999));
}

void
test::cnv::force_in_type()
{
    boost::cnv::cstringstream cnv;

    string const      s1 = boost::convert<string>(-1, cnv).value();
    string const      s2 = boost::convert<string, unsigned int>(-1, cnv).value();
    char const* expected = sizeof(unsigned int) == 4 ? "4294967295" : 0;

    if (expected)
    {
        BOOST_TEST(s1 == "-1");
        BOOST_TEST(s2 == expected);
    }
}

static
test::cnv::strings
prepare_strs(test::cnv::ints const& ints)
{
    test::cnv::strings strs;
    int const          size = ints.size();

    strs.reserve(size);

    for (int k = 0; k < size; ++k)
    {
        int    this_i = ints[k];
        string this_s = boost::convert<string>(this_i, boost::cnv::lexical_cast()).value();
        int    back_i = boost::convert<int>(this_s, boost::cnv::spirit()).value();

        BOOST_ASSERT(this_i == back_i);

        strs.push_back(this_s);
    }
    return strs;
}

int
main(int argc, char const* argv[])
{
    BOOST_TEST(test::performance::spirit_framework());

    test::cnv::ints const    ints = test::cnv::prepare_ints(10000);
    test::cnv::strings const strs = prepare_strs(ints);

    example::getting_started();
    example::getting_serious();
    example::algorithms();

    test::cnv::is_converter();
    test::cnv::invalid(boost::cnv::lexical_cast());
//    test::cnv::invalid(boost::cnv::cstringstream());
//    test::cnv::invalid(boost::cnv::strtol());
//    test::cnv::invalid(boost::cnv::printf());
    test::cnv::scratchpad();
    test::cnv::sfinae();
    test::cnv::int_to_string();
    test::cnv::string_to_bool();
    test::cnv::str_to_int(boost::cnv::lexical_cast());
    test::cnv::str_to_int(boost::cnv::cstringstream());
    test::cnv::str_to_int(boost::cnv::strtol());
    test::cnv::str_to_int(boost::cnv::printf());
    test::cnv::type_to_string(boost::cnv::printf());
    test::cnv::string_to_type(boost::cnv::strtol()); 
    test::cnv::string_to_type(boost::cnv::printf());
    test::cnv::type_to_string(boost::cnv::printf());
    test::cnv::user_type();
    test::cnv::force_in_type();
    test::cnv::lcast_converter();
    test::cnv::sstream();
    test::cnv::algorithms();
    test::cnv::callables();
    test::cnv::encryption();
    test::cnv::performance(strs, ints);
    test::cnv::spirit(strs);

    return boost::report_errors();
}
