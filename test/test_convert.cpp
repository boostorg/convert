// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "./str_to_int.hpp"
#include "./invalid.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using std::string;
using std::wstring;

template<typename Converter>
void
test::type_to_string(Converter const& cnv)
{
    BOOST_TEST("255" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::string_to_type(Converter const& cnv)
{
    BOOST_TEST( 255 == boost::convert<int>("255", cnv(arg::base = boost::cnv::base::dec)).value_or(999));
    BOOST_TEST( 999 == boost::convert<int>( "FF", cnv(arg::base = boost::cnv::base::dec)).value_or(999));
    BOOST_TEST( 255 == boost::convert<int>( "FF", cnv(arg::base = boost::cnv::base::hex)).value_or(999));
    BOOST_TEST( 173 == boost::convert<int>("255", cnv(arg::base = boost::cnv::base::oct)).value_or(999));
    BOOST_TEST( 999 != boost::convert<double>("1.23", cnv).value_or(999));
}

void
test::force_in_type()
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

int
main(int argc, char const* argv[])
{
    test::ints                                ints;
    test::strings                          strings;
    boost::random::mt19937                     gen;
    boost::random::uniform_int_distribution<> dist (INT_MIN, INT_MAX);

    ints   .reserve(test::num_cycles);
    strings.reserve(test::num_cycles);

    for (int k = 0; k < test::num_cycles; ++k)
    {
        int v = dist(gen);

        ints   .push_back(v);
        strings.push_back(boost::lexical_cast<std::string>(v));
    }
    example::getting_started();
    example::getting_serious();

    test::invalid(boost::cnv::lexical_cast());
//    test::invalid(boost::cnv::cstringstream());
//    test::invalid(boost::cnv::strtol());
//    test::invalid(boost::cnv::printf());
    test::scratchpad();
    test::sfinae();
    test::int_to_string();
    test::string_to_bool();
    test::str_to_int(boost::cnv::lexical_cast());
    test::str_to_int(boost::cnv::cstringstream());
    test::str_to_int(boost::cnv::strtol());
    test::str_to_int(boost::cnv::printf());
    test::type_to_string(boost::cnv::printf());
    test::string_to_type(boost::cnv::strtol()); 
    test::string_to_type(boost::cnv::printf());
    test::type_to_string(boost::cnv::printf());
    test::user_type();
    test::force_in_type();
    test::lcast_converter();
    test::sstream();
    test::algorithms();
    test::callables();
    test::encryption();
    test::performance(strings, ints);
    test::spirit(strings);

    return boost::report_errors();
}
