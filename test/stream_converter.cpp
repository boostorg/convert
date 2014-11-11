// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cstdio>

//[stream_using
using std::string;
using std::wstring;
using boost::convert;
//]
//[stream_cnv_namespace_shortcut
namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;
//]

static
void
test_dbl_to_str()
{
    boost::cnv::cstream cnv;

    cnv(std::fixed);

    BOOST_TEST("100.00" == convert<string>( 99.999, cnv(arg::precision = 2)).value());
    BOOST_TEST( "99.95" == convert<string>( 99.949, cnv(arg::precision = 2)).value());
    BOOST_TEST("-99.95" == convert<string>(-99.949, cnv(arg::precision = 2)).value());
    BOOST_TEST(  "99.9" == convert<string>( 99.949, cnv(arg::precision = 1)).value());
    BOOST_TEST(  "1.00" == convert<string>(  0.999, cnv(arg::precision = 2)).value());
    BOOST_TEST( "-1.00" == convert<string>( -0.999, cnv(arg::precision = 2)).value());
    BOOST_TEST(  "0.95" == convert<string>(  0.949, cnv(arg::precision = 2)).value());
    BOOST_TEST( "-0.95" == convert<string>( -0.949, cnv(arg::precision = 2)).value());
    BOOST_TEST(   "1.9" == convert<string>(  1.949, cnv(arg::precision = 1)).value());
    BOOST_TEST(  "-1.9" == convert<string>( -1.949, cnv(arg::precision = 1)).value());
}

static
void
test_numbase()
{
    //[stream_numbase_example1
    /*`The following example demonstrates the deployment of `std::dec`, `std::oct` `std::hex`
       manipulators:
     */
    boost::cnv::cstream ccnv;

    BOOST_TEST(17 == convert<int>( "11", ccnv(std::hex)).value()); // 11(16) = 17(10)
    BOOST_TEST( 9 == convert<int>( "11", ccnv(std::oct)).value()); // 11(8)  = 9(10)
    BOOST_TEST(11 == convert<int>( "11", ccnv(std::dec)).value());

    BOOST_TEST( "12" == convert<string>(18, ccnv(std::hex)).value()); // 18(10) = 12(16)
    BOOST_TEST( "12" == convert<string>(10, ccnv(std::oct)).value()); // 10(10) = 12(8)
    BOOST_TEST( "12" == convert<string>(12, ccnv(std::dec)).value());
    BOOST_TEST("377" == convert<string>(255, ccnv(arg::base = boost::cnv::base::oct)).value());
    BOOST_TEST( "ff" == convert<string>(255, ccnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("255" == convert<string>(255, ccnv(arg::base = boost::cnv::base::dec)).value());

    ccnv(std::showbase);

    BOOST_TEST("0x12" == convert<string>(18, ccnv(std::hex)).value());
    BOOST_TEST( "012" == convert<string>(10, ccnv(std::oct)).value());

    ccnv(std::uppercase);

    BOOST_TEST("0X12" == convert<string>(18, ccnv(std::hex)).value());
    //]
    //[stream_numbase_example2
    BOOST_TEST(17 == convert<int>("11", ccnv(arg::base = cnv::base::hex)).value());
    BOOST_TEST( 9 == convert<int>("11", ccnv(arg::base = cnv::base::oct)).value());
    BOOST_TEST(11 == convert<int>("11", ccnv(arg::base = cnv::base::dec)).value());
    //]
    //[wide_stream_numeric_base
    boost::cnv::wstream wcnv;

    BOOST_TEST(17 == convert<int>(L"11", wcnv(std::hex)).value()); // 11(16) = 17(10)
    BOOST_TEST( 9 == convert<int>(L"11", wcnv(std::oct)).value()); // 11(8)  = 9(10)
    BOOST_TEST(11 == convert<int>(L"11", wcnv(std::dec)).value());

    BOOST_TEST(L"254" == convert<wstring>(254, wcnv(arg::base = cnv::base::dec)).value());
    BOOST_TEST( L"fe" == convert<wstring>(254, wcnv(arg::base = cnv::base::hex)).value());
    BOOST_TEST(L"376" == convert<wstring>(254, wcnv(arg::base = cnv::base::oct)).value());
    //]
}

static
void
test_boolalpha()
{
    boost::cnv::cstream cnv;
    //[stream_boolalpha_example
    BOOST_TEST(convert<string>( true, cnv(std::boolalpha)).value() ==  "true");
    BOOST_TEST(convert<string>(false, cnv(std::boolalpha)).value() == "false");

    BOOST_TEST(convert<bool>( "true", cnv(std::boolalpha)).value() ==  true);
    BOOST_TEST(convert<bool>("false", cnv(std::boolalpha)).value() == false);

    BOOST_TEST(convert<string>( true, cnv(std::noboolalpha)).value() == "1");
    BOOST_TEST(convert<string>(false, cnv(std::noboolalpha)).value() == "0");

    BOOST_TEST(convert<bool>("1", cnv(std::noboolalpha)).value() ==  true);
    BOOST_TEST(convert<bool>("0", cnv(std::noboolalpha)).value() == false);
    //]
}

static
void
test_skipws()
{
    //[stream_skipws1_example
    boost::cnv::cstream    ccnv;
    char const* const cstr_good = "  123";
    char const* const  cstr_bad = "  123 "; // std::skipws only affects leading spaces.

    ccnv(std::skipws); // Skip leading whitespaces

    BOOST_TEST(  123 == convert<int>(cstr_good, ccnv).value());
    BOOST_TEST("123" == convert<string>("  123", ccnv).value());

    BOOST_TEST(!convert<int>(cstr_bad, ccnv));

    ccnv(std::noskipws); // Do not ignore leading whitespaces

    // All conversions fail.
    BOOST_TEST(!convert<int>(cstr_good, ccnv));
    BOOST_TEST(!convert<int>( cstr_bad, ccnv));
    //]
    //[stream_skipws2_example
    ccnv(arg::skipws = true); // Skip leading whitespaces

    BOOST_TEST(  123 == convert<int>(cstr_good, ccnv).value());
    BOOST_TEST("123" == convert<string>(cstr_good, ccnv).value());
    //]
    //[wide_stream_skipws
    boost::cnv::wstream       wcnv;
    wchar_t const* const wstr_good = L"  123";
    wchar_t const* const  wstr_bad = L"  123 ";

    wcnv(std::skipws);

    BOOST_TEST(123 == convert<int>(wstr_good, wcnv).value());
    BOOST_TEST(!convert<int>(wstr_bad, wcnv));

    wcnv(std::noskipws);

    BOOST_TEST(!convert<int>(wstr_good, wcnv));
    BOOST_TEST(!convert<int>( wstr_bad, wcnv));
    //]
}

static
void
test_width()
{
    //[stream_width_example
    boost::cnv::cstream cnv;

    string s01 = convert<string>(12, cnv(std::setw(4))).value();
    string s02 = convert<string>(12, cnv(std::setw(5))(std::setfill('*'))).value();
    string s03 = convert<string>(12, cnv(std::setw(5))(std::setfill('*'))(std::left)).value();

    BOOST_TEST(s01 == "  12");  // Field width = 4.
    BOOST_TEST(s02 == "***12"); // Field width = 5, filler = '*'.
    BOOST_TEST(s03 == "12***"); // Field width = 5, filler = '*', left adjustment

    /*`It needs to be remembered that `boost::cnv::stream` converter uses `std::stream` as its underlying
       conversion engine. Consequently, formatting-related behavior are driven by the `std::stream`. Namely,
       after every operation is performed, the ['default field width is restored]. The values of
       the fill character and the adjustment remain unchanged until they are modified explicitly.
     */

    // The fill and adjustment remain '*' and 'left'.
    string s11 = convert<string>(12, cnv(arg::width = 4)).value();
    string s12 = convert<string>(12, cnv(arg::width = 5)
                                        (arg::fill = ' ')
                                        (arg::adjust = cnv::adjust::right)).value();

    BOOST_TEST(s11 == "12**");  // Field width was set to 4.
    BOOST_TEST(s12 == "   12"); // Field width was set to 5 with the ' ' filler.
    //]
}

static
void
test_manipulators()
{
    boost::cnv::cstream ccnv;
    boost::cnv::wstream wcnv;

    int const hex_v01 = boost::convert<int>("FF", ccnv(std::hex)).value_or(0);
    int const hex_v02 = boost::convert<int>(L"F", wcnv(std::hex)).value_or(0);
    int const hex_v03 = boost::convert<int>("FF", ccnv(std::dec)).value_or(-5);
    int const hex_v04 = boost::convert<int>(L"F", wcnv(std::dec)).value_or(-6);

    BOOST_TEST(hex_v01 == 255); // "FF"
    BOOST_TEST(hex_v02 ==  15); // L"F"
    BOOST_TEST(hex_v03 ==  -5); // Failed conversion
    BOOST_TEST(hex_v04 ==  -6); // Failed conversion

    ccnv(std::noshowbase)(std::nouppercase)(std::oct);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "17");

    ccnv(std::showbase);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "017");

    ccnv(std::uppercase)(std::hex);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "0XF");

    ccnv(std::noshowbase)(std::nouppercase)(std::oct);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "17");

    ccnv(std::showbase)(arg::uppercase = true)(arg::base = cnv::base::hex);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "0XF");
}

void
test_locale_example()
{
    //[stream_locale_example1
    boost::cnv::cstream cnv;
    std::locale  rus_locale;
    std::locale  eng_locale;

    char const* eng_locale_name = test::cnv::is_msc ? "English_United States.1251" : "en_US.UTF-8";
    char const* rus_locale_name = test::cnv::is_msc ? "Russian_Russia.1251" : "ru_RU.UTF-8";
    char const*    rus_expected = test::cnv::is_msc ? "1,235e-002" : "1,235e-02";
    char const*    eng_expected = test::cnv::is_msc ? "1.235e-002" : "1.235e-02";
    char const*      double_s01 = test::cnv::is_msc ? "1.2345E-002" : "1.2345E-02";

//  cnv(std::setprecision(4))(std::uppercase)(std::scientific);
    cnv(arg::precision = 4)
       (arg::uppercase = true)
       (arg::notation = cnv::notation::scientific);

    double double_v01 = convert<double>(double_s01, cnv).value();
    string double_s02 = convert<string>(double_v01, cnv).value();

    BOOST_TEST(double_s01 == double_s02);

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Bad locale %s.\n", rus_locale_name); exit(1); }

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Bad locale %s.\n", eng_locale_name); exit(1); }

//  cnv(std::setprecision(3))(std::nouppercase);
    cnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = convert<string>(double_v01, cnv(rus_locale)).value();
    string double_eng = convert<string>(double_v01, cnv(eng_locale)).value();

    BOOST_TEST(double_rus == rus_expected);
    BOOST_TEST(double_eng == eng_expected);
    //]
}

static
void
test_locale()
{
    boost::cnv::cstream     cnv;
    std::locale      rus_locale;
    std::locale      eng_locale;
    bool             eng_ignore = false;
    bool             rus_ignore = false;
    char const* eng_locale_name = test::cnv::is_msc ? "English_United States.1251" : "en_US.UTF-8";
    char const* rus_locale_name = test::cnv::is_msc ? "Russian_Russia.1251" : "ru_RU.UTF-8";
    char const*    eng_expected = test::cnv::is_msc ? "1.235e-002" : "1.235e-02";
    char const*    rus_expected = test::cnv::is_msc ? "1,235e-002" : "1,235e-02";
    char const*      double_s01 = test::cnv::is_msc ? "1.2345E-002" : "1.2345E-02";

    cnv(arg::precision = 4)
       (arg::uppercase = true)
       (arg::notation = cnv::notation::scientific);

    double double_v01 = convert<double>(double_s01, cnv).value();
    string double_s02 = convert<string>(double_v01, cnv).value();

    BOOST_TEST(double_s01 == double_s02);

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Bad locale %s. Ignored.\n", eng_locale_name); eng_ignore = true; }

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Bad locale %s. Ignored.\n", rus_locale_name); rus_ignore = true; }

//  cnv(std::setprecision(3))(std::nouppercase);
    cnv(arg::precision = 3)(arg::uppercase = false);

    if (!eng_ignore) BOOST_TEST(eng_expected == convert<string>(double_v01, cnv(eng_locale)).value());
    if (!rus_ignore) BOOST_TEST(rus_expected == convert<string>(double_v01, cnv(rus_locale)).value());
}

static
void
test_user_str()
{
    //[stream_my_string
    boost::cnv::cstream cnv;
    my_string        my_str("123");

    cnv(std::setprecision(2))(std::fixed);

    BOOST_TEST(123 == convert<int>(my_str, cnv).value());

    BOOST_TEST("100.00" == convert<my_string>( 99.999, cnv).value());
    BOOST_TEST( "99.95" == convert<my_string>( 99.949, cnv).value());
    BOOST_TEST("-99.95" == convert<my_string>(-99.949, cnv).value());
    //]
}

void
test::cnv::stream_converter()
{
    test_numbase();
    test_boolalpha();
    test_skipws();
    test_width();
    test_manipulators();
    test_locale();
    test_dbl_to_str();
    test_user_str();
}
