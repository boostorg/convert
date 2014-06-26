// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;
using boost::convert;

namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;

static
void
test_width()
{
    boost::cnv::strtol cnv;

    string s01 = convert<string>( 12, cnv(arg::width = 4)).value();
    string s02 = convert<string>( 12, cnv(arg::width = 5)
                                         (arg::fill = '*')).value();
    string s03 = convert<string>( 12, cnv(arg::width = 5)
                                         (arg::fill = 'x')
                                         (arg::adjustment = cnv::adjustment::left)).value();
    string s04 = convert<string>(-98, cnv(arg::width = 6)
                                         (arg::fill = 'Z')
                                         (arg::adjustment = cnv::adjustment::right)).value();

    BOOST_TEST(s01 == "  12");
    BOOST_TEST(s02 == "***12");
    BOOST_TEST(s03 == "12xxx");
    BOOST_TEST(s04 == "ZZZ-98");
}

static
void
test_base()
{
    boost::cnv::strtol cnv;

    BOOST_TEST("255" == convert<string>(255, cnv(arg::base = boost::cnv::base::dec)).value());
    BOOST_TEST( "FF" == convert<string>(255, cnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("377" == convert<string>(255, cnv(arg::base = boost::cnv::base::oct)).value());
}

static
void
test_int_to_str()
{
    test_base();
    test_width();
}

static
void
test_dbl_to_str()
{
    struct double_desc
    {
        double value;
        int precision;
    };
    double_desc doubles[] =
    {
        { 99.999, 2 },
        { 99.949, 2 },
        {-99.949, 2 },
        { 99.949, 1 },
        {  0.999, 2 },
        { -0.999, 2 },
        {  0.949, 2 },
        { -0.949, 2 },
        {  1.949, 1 },
        { -1.949, 1 }
    };
    int const             sz = sizeof(doubles) / sizeof(doubles[0]);
    boost::cnv::strtol  cnv1;
    boost::cnv::cstream cnv2;

    cnv2(std::fixed);

    for (int k = 0; k < sz; ++k)
    {
        string str1 = convert<string>(doubles[k].value, cnv1(arg::precision = doubles[k].precision)).value();
        string str2 = convert<string>(doubles[k].value, cnv2(arg::precision = doubles[k].precision)).value();

        BOOST_TEST(str1 == str2);
    }
}

static
void
test_str_to_int()
{
    boost::cnv::strtol cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";

    BOOST_TEST( -1 == convert<int>(not_int_str, cnv).value_or(-1));
    BOOST_TEST(-11 == convert<int>(std_str,     cnv).value_or(-1));
    BOOST_TEST(-12 == convert<int>(c_str,       cnv).value_or(-1));
}

void
test::cnv::strtol_converter()
{
    test_str_to_int();
    test_int_to_str();
    test_dbl_to_str();
}
