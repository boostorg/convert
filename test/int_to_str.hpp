// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_TEST_INT_TO_STR_HPP
#define BOOST_CONVERT_TEST_INT_TO_STR_HPP

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;

template<typename Converter>
void
test::cnv::int_to_str(Converter const& cnv)
{
    string const    not_int_str = "not an int";
    string const        std_str = "-11";
    char const* const     c_str = "-12";
    char const      array_str[] = "-15";

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s01 = boost::convert< string>(-1, cnv).value_or(std_str);
    string                    s02 = boost::convert< string>(-2, cnv).value_or(c_str);
    string                    s05 = boost::convert< string>(-5, cnv).value_or(array_str);
    boost::optional< string> rs01 = boost::convert< string>(-1, cnv);
    boost::optional< string> rs02 = boost::convert< string>(-2, cnv);
    boost::optional< string> rs05 = boost::convert< string>(-5, cnv);

    BOOST_TEST(s01 ==  "-1"); BOOST_TEST(rs01 && rs01.value() ==  "-1");
    BOOST_TEST(s02 ==  "-2"); BOOST_TEST(rs02 && rs02.value() ==  "-2");
    BOOST_TEST(s05 ==  "-5"); BOOST_TEST(rs05 && rs05.value() ==  "-5");

    string                    s11 = boost::convert< string>(-1, cnv).value();
    boost::optional< string> rs11 = boost::convert< string>(-1, cnv);

    BOOST_TEST( s11 ==  "-1");
    BOOST_TEST(rs11 && rs11.value() ==  "-1");
}

#endif // BOOST_CONVERT_TEST_INT_TO_STR_HPP
