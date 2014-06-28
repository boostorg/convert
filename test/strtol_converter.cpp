// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/stream.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_01.hpp>

//[strtol_basic_deployment_header
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>

using std::string;
using boost::convert;

struct boost::cnv::by_default : public boost::cnv::strtol {};
//]

static
void
test_str_to_int()
{
    //[strtol_basic_deployment
    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";

    BOOST_TEST( -1 == convert<int>(not_int_str).value_or(-1));
    BOOST_TEST(-11 == convert<int>(    std_str).value_or(-1));
    BOOST_TEST(-12 == convert<int>(      c_str).value_or(-1));
    //]
}

//[strtol_numeric_base_header
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>

using std::string;
using boost::convert;

namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;
//]
static
void
test_width()
{
    //[strtol_width
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
    //]
}

static
void
test_base()
{
    //[strtol_numeric_base
    boost::cnv::strtol cnv;

    BOOST_TEST("255" == convert<string>(255, cnv(arg::base = boost::cnv::base::dec)).value());
    BOOST_TEST( "FF" == convert<string>(255, cnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("377" == convert<string>(255, cnv(arg::base = boost::cnv::base::oct)).value());
    //]
}

static
void
dbl_to_str_example()
{
    //[strtol_precision
    boost::cnv::strtol cnv;

    BOOST_TEST(  "12.3" == convert<string>(12.3456, cnv(arg::precision = 1)).value());
    BOOST_TEST( "12.35" == convert<string>(12.3456, cnv(arg::precision = 2)).value());
    BOOST_TEST("12.346" == convert<string>(12.3456, cnv(arg::precision = 3)).value());

    BOOST_TEST(  "-12.3" == convert<string>(-12.3456, cnv(arg::precision = 1)).value());
    BOOST_TEST( "-12.35" == convert<string>(-12.3456, cnv(arg::precision = 2)).value());
    BOOST_TEST("-12.346" == convert<string>(-12.3456, cnv(arg::precision = 3)).value());
    //]
}

static
std::pair<double, int>
get_random()
{
    static boost::random::mt19937                          gen (::time(0));
    static boost::random::uniform_int_distribution<> precision (0, 8);
    static boost::random::uniform_int_distribution<>  int_part (0, INT_MAX); // INT_MAX(32) = 2,147,483,647
    static boost::random::uniform_01<double>          fraction; // uniform double in [0,1)
    static bool                                           sign;

    double dbl = (int_part(gen) + fraction(gen)) * ((sign = !sign) ? 1 : -1);

//  printf("%.12f\n", dbl);

    return std::make_pair(dbl, precision(gen));
}

static
void
test_dbl_to_str()
{
    double      round_up_abs01 = ::rint(-0.5);
    double      round_up_abs02 = ::round(-0.5);
    double      round_up_abs11 = ::rint(0.5);
    double      round_up_abs12 = ::round(0.5);
    int const      num_tries = 1000000;
    boost::cnv::strtol  cnv1;
    boost::cnv::cstream cnv2;
    double huge_v = 987654321098765432109.123;

    cnv2(std::fixed);

    printf("cnv::strtol::%s: started with %d random numbers...\n", __FUNCTION__, num_tries);
    printf("%f\n", huge_v);

    string huge = convert<string>(huge_v, cnv1(arg::precision = 2)).value();

    printf("%s\n", huge.c_str());

    for (int k = 0; k < num_tries; ++k)
    {
        std::pair<double, int> random = get_random();
        double                    dbl = random.first;
        int                 precision = random.second;
        string                   str1 = convert<string>(dbl, cnv1(arg::precision = precision)).value();
        string                   str2 = convert<string>(dbl, cnv2(arg::precision = precision)).value();

        if (str1 != str2)
            printf("dbl=%.12f(%d).\t(%s)\t(%.*f)\t(%s)\n",
                   dbl, precision,
                   str1.c_str(),
                   precision, dbl,
                   str2.c_str());
    }
    BOOST_TEST(    "0" == convert<string>(    0.0, cnv1(arg::precision = 0)).value());
    BOOST_TEST(  "0.0" == convert<string>(    0.0, cnv1(arg::precision = 1)).value());
    BOOST_TEST( "0.00" == convert<string>(    0.0, cnv1(arg::precision = 2)).value());
    BOOST_TEST(    "1" == convert<string>(   0.95, cnv1(arg::precision = 0)).value());
    BOOST_TEST(  "1.0" == convert<string>(   0.95, cnv1(arg::precision = 1)).value());
    BOOST_TEST(  "0.9" == convert<string>(   0.94, cnv1(arg::precision = 1)).value());
    BOOST_TEST(    "1" == convert<string>(   1.05, cnv1(arg::precision = 0)).value());
    BOOST_TEST(  "1.1" == convert<string>(   1.05, cnv1(arg::precision = 1)).value());
    BOOST_TEST("0.123" == convert<string>( 0.1234, cnv1(arg::precision = 3)).value());
    BOOST_TEST("0.124" == convert<string>( 0.1235, cnv1(arg::precision = 3)).value());
    BOOST_TEST(   "10" == convert<string>(  9.654, cnv1(arg::precision = 0)).value());
    BOOST_TEST(  "902" == convert<string>(901.805, cnv1(arg::precision = 0)).value());

    printf("cnv::strtol::%s: finished.\n", __FUNCTION__);
}

void
test::cnv::strtol_converter()
{
    test_str_to_int();
    test_base();
    test_width();
    test_dbl_to_str();
    dbl_to_str_example();
}
