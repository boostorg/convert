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

int
main(int argc, char const* argv[])
{
    test::cnv::ints                                ints;
    test::cnv::strings                             strs;
    boost::random::mt19937                     gen;
    boost::random::uniform_int_distribution<> dist (INT_MIN, INT_MAX);

    ints.reserve(test::cnv::num_cycles);
    strs.reserve(test::cnv::num_cycles);

    for (int k = 0; k < test::cnv::num_cycles; ++k)
    {
        int    iv = dist(gen);
        string sv = boost::lexical_cast<std::string>(iv);

        ints.push_back(iv);
        strs.push_back(sv);
    }
    example::getting_started();
    example::getting_serious();
    example::algorithms();

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
