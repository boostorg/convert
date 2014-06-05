// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

void
test::lcast_converter()
{
    boost::cnv::lexical_cast cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";
    int const            v00 = boost::convert<int>(not_int_str, cnv).value_or(-1);
    int const            v01 = boost::convert<int>(std_str,     cnv).value_or(-1);
    int const            v02 = boost::convert<int>(c_str,       cnv).value_or(-1);

    BOOST_TEST(v00 ==  -1); // Failed conversion. No throw
    BOOST_TEST(v01 == -11);
    BOOST_TEST(v02 == -12);
}
