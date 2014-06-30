// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost/convert.hpp>
#include <boost/convert/printf.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "./test.hpp"

using std::string;
using boost::convert;

namespace arg = boost::cnv::parameter;

namespace { namespace local
{
    void str_to_int();
    void int_to_str();
}}

void
local::str_to_int()
{
    boost::cnv::printf cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";

    BOOST_TEST( -1 == convert<int>(not_int_str, cnv).value_or(-1));
    BOOST_TEST(-11 == convert<int>(std_str,     cnv).value_or(-1));
    BOOST_TEST(-12 == convert<int>(c_str,       cnv).value_or(-1));
}

void
local::int_to_str()
{
    boost::cnv::printf cnv;

    BOOST_TEST("255" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value());
    BOOST_TEST( "ff" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("377" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value());

    string s01 = convert<string>(12.3456, cnv(arg::precision = 6)).value();
    string s02 = convert<string>(12.3456, cnv(arg::precision = 3)).value();

    BOOST_TEST(s01 == "12.345600");
    BOOST_TEST(s02 == "12.346");
}

void
test::cnv::printf_converter()
{
    local::str_to_int();
    local::int_to_str();
}
