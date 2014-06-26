// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/stream.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

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

    string s05 = convert<string>(-12.3451, cnv(arg::precision = 2)
                                              (arg::width = 10)
                                              (arg::fill = '*')
                                              (arg::adjustment = cnv::adjustment::left)).value();
    string s06 = convert<string>(-12.3450, cnv(arg::adjustment = cnv::adjustment::right)).value();

    BOOST_TEST(s01 == "  12");
    BOOST_TEST(s02 == "***12");
    BOOST_TEST(s03 == "12xxx");
    BOOST_TEST(s04 == "ZZZ-98");
    BOOST_TEST(s05 == "-12.35****");
    BOOST_TEST(s06 == "****-12.35");
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
int
get_precision()
{
    static boost::random::mt19937                     gen (::time(0));
    static boost::random::uniform_int_distribution<> dist (0, 8);

    return dist(gen);
}

static
double
get_double()
{
    static boost::random::mt19937                      gen (::time(0));
    static double const                                max (1000000);
    static boost::random::uniform_int_distribution<> dist1 (0, INT_MAX); // INT_MAX(32) = 2,147,483,647
    static boost::random::uniform_int_distribution<> dist2 (0, max - 1);
    static bool                                sign = false;

    return (double(dist1(gen)) + double(dist2(gen)) / max) * ((sign = !sign) ? 1 : -1);
}

static
void
test_dbl_to_str()
{
    printf("cnv::strtol::%s: started...\n", __FUNCTION__);

    int const     num_cycles = 1000000;
    boost::cnv::strtol  cnv1;
    boost::cnv::cstream cnv2;

    cnv2(std::fixed);

    for (int k = 0; k < num_cycles; ++k)
    {
        double    dbl = get_double();
        int precision = get_precision();
        string   str1 = convert<string>(dbl, cnv1(arg::precision = precision)).value();
        string   str2 = convert<string>(dbl, cnv2(arg::precision = precision)).value();

        if (str1 != str2)
            printf("dbl=%.10f (%d). <%s><%s>\n", dbl, precision, str1.c_str(), str2.c_str());
    }
    printf("cnv::strtol::%s: finished.\n", __FUNCTION__);
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
