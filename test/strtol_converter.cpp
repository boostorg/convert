// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;
using boost::convert;

namespace arg = boost::cnv::parameter;

void
test::cnv::strtol_converter()
{
    boost::cnv::strtol cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";

    BOOST_TEST(   -1 == convert<int>(not_int_str, cnv).value_or(-1));
    BOOST_TEST(  -11 == convert<int>(std_str,     cnv).value_or(-1));
    BOOST_TEST(  -12 == convert<int>(c_str,       cnv).value_or(-1));

    BOOST_TEST("255" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value());
    BOOST_TEST( "FF" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("377" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value());
}
