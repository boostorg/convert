// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./example.hpp"
//[lexical_cast_headers1
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>

using std::string;
//]

void
example::lexical_cast()
{
    //[lexical_cast_example1

    boost::cnv::lexical_cast cnv;

    int    i1 = boost::lexical_cast<int>("123"); // Throws if the conversion fails
    int    i2 = boost::convert<int>("123", cnv).value(); // Throws if the conversion fails
    int    i3 = boost::convert<int>("uhm", cnv).value_or(-1); // Returns -1 if the conversion fails
    string s1 = boost::lexical_cast<string>(123);
    string s2 = boost::convert<string>(123, cnv).value();

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    BOOST_TEST(s1 == "123");
    BOOST_TEST(s2 == "123");

    //]
}
