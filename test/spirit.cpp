// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cstdio>

using std::string;
using boost::convert;

namespace arg = boost::cnv::parameter;

struct boost::cnv::by_default : public boost::cnv::spirit {};

void
test::cnv::spirit_converter()
{
    boost::cnv::spirit cnv;

    boost::optional<int>    i1 = convert<int>("uhm");
    boost::optional<int>    i2 = convert<int>("123");
    boost::optional<int>    i3 = convert<int>("0x23", cnv(arg::base = boost::cnv::base::hex));
//    boost::optional<double> d1 = convert<double>("12.uhm");
//    boost::optional<double> d2 = convert<double>("12.3");
    boost::optional<string> s1 = convert<string>(123456789012345678);
//    boost::optional<string> s2 = convert<string>(12.34);

    BOOST_TEST(!i1);
    BOOST_TEST( i2 && i2.value() == 123);
    BOOST_TEST( i3 && i3.value() == 35);
//    BOOST_TEST(!d1);
//    BOOST_TEST( d2 && d2.value() == 12.3);
    BOOST_TEST( s1);
    BOOST_TEST( s1 && s1.value() == "123456789012345678");
//    BOOST_TEST( s2 && s2.value() == "12.34");

//    printf("%s\n", s2->c_str());
}
