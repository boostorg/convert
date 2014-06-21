// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "./str_to_int.hpp"

namespace arg = boost::cnv::parameter;

using std::string;
using std::wstring;

template<typename Converter>
void
test::cnv::type_to_str(Converter const& cnv)
{
    BOOST_TEST("255" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::cnv::str_to_type(Converter const& cnv)
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
    boost::cnv::cstream cnv;

    string const      s1 = boost::convert<string>(-1, cnv).value();
    string const      s2 = boost::convert<string, unsigned int>(-1, cnv).value();
    char const* expected = sizeof(unsigned int) == 4 ? "4294967295" : 0;

    if (expected)
    {
        BOOST_TEST(s1 == "-1");
        BOOST_TEST(s2 == expected);
    }
}

int
main(int argc, char const* argv[])
{
    example::getting_started();
    example::getting_serious();
    example::algorithms();
    example::stream();
    example::lexical_cast();
    example::default_converter();

    test::cnv::scratchpad();
    test::cnv::is_converter();
    test::cnv::stream();

    BOOST_TEST(test::performance::spirit_framework());

    test::cnv::sfinae();
    test::cnv::int_to_string();
    test::cnv::string_to_bool();
    test::cnv::str_to_int(boost::cnv::lexical_cast());
    test::cnv::str_to_int(boost::cnv::cstream());
    test::cnv::str_to_int(boost::cnv::strtol());
    test::cnv::str_to_int(boost::cnv::printf());
    test::cnv::type_to_str(boost::cnv::printf());
    test::cnv::str_to_type(boost::cnv::strtol()); 
    test::cnv::str_to_type(boost::cnv::printf());
    test::cnv::type_to_str(boost::cnv::printf());
    test::cnv::user_type();
    test::cnv::force_in_type();
    test::cnv::lcast_converter();
    test::cnv::algorithms();
    test::cnv::callables();
    test::cnv::fallbacks();
    test::cnv::encryption();
    test::cnv::performance();
    test::cnv::spirit();

    return boost::report_errors();
}
