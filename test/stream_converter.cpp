// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/detail/lightweight_test.hpp>

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
test_numbase()
{
    //[stream_numbase_example1
    /*`The following example demonstrates the deployment of `std::dec`, `std::oct` `std::hex`
       manipulators:
     */
    boost::cnv::cstream ccnv;
    boost::cnv::wstream wcnv;

    BOOST_TEST(convert<int>( "11", ccnv(std::hex)).value() == 17); // 11(16) = 17(10)
    BOOST_TEST(convert<int>(L"11", wcnv(std::hex)).value() == 17); // 11(16) = 17(10)
    BOOST_TEST(convert<int>( "11", ccnv(std::oct)).value() ==  9); // 11(8)  = 9(10)
    BOOST_TEST(convert<int>(L"11", wcnv(std::oct)).value() ==  9); // 11(8)  = 9(10)
    BOOST_TEST(convert<int>( "11", ccnv(std::dec)).value() == 11);
    BOOST_TEST(convert<int>(L"11", wcnv(std::dec)).value() == 11);

    BOOST_TEST(convert<string>(18, ccnv(std::hex)).value() == "12"); // 18(10) = 12(16)
    BOOST_TEST(convert<string>(10, ccnv(std::oct)).value() == "12"); // 10(10) = 12(8)
    BOOST_TEST(convert<string>(12, ccnv(std::dec)).value() == "12");

    ccnv(std::showbase);

    BOOST_TEST(convert<string>(18, ccnv(std::hex)).value() == "0x12");
    BOOST_TEST(convert<string>(10, ccnv(std::oct)).value() == "012");

    ccnv(std::uppercase);

    BOOST_TEST(convert<string>(18, ccnv(std::hex)).value() == "0X12");
    //]
    //[stream_numbase_example2
    BOOST_TEST(convert<int>("11", ccnv(arg::base = cnv::base::hex)).value() == 17);
    BOOST_TEST(convert<int>("11", ccnv(arg::base = cnv::base::oct)).value() ==  9);
    BOOST_TEST(convert<int>("11", ccnv(arg::base = cnv::base::dec)).value() == 11);
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
    boost::cnv::cstream       ccnv;
    boost::cnv::wstream       wcnv;
    char const* const    cstr_good = "  123";
    char const* const     cstr_bad = "  123 "; // std::skipws only affects leading spaces.
    wchar_t const* const wstr_good = L"  123";
    wchar_t const* const  wstr_bad = L"  123 ";

    ccnv(std::skipws); // Skip leading whitespaces
    wcnv(std::skipws);

    BOOST_TEST( convert<int>(cstr_good, ccnv).value() == 123);
    BOOST_TEST(!convert<int>( cstr_bad, ccnv));
    BOOST_TEST( convert<int>(wstr_good, wcnv).value() == 123);
    BOOST_TEST(!convert<int>( wstr_bad, wcnv));

    ccnv(std::noskipws); // Do not ignore leading whitespaces
    wcnv(std::noskipws);

    // All conversions fail.
    BOOST_TEST(!convert<int>(cstr_good, ccnv));
    BOOST_TEST(!convert<int>( cstr_bad, ccnv));
    BOOST_TEST(!convert<int>(wstr_good, wcnv));
    BOOST_TEST(!convert<int>( wstr_bad, wcnv));
    //]
    //[stream_skipws2_example
    ccnv(arg::skipws = true); // Skip leading whitespaces

    BOOST_TEST(convert<int>(cstr_good, ccnv).value() == 123);
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

    BOOST_TEST(s01 == "  12");  // Field width was set to 4.
    BOOST_TEST(s02 == "***12"); // Field width was set to 5 with the '*' filler.
    BOOST_TEST(s03 == "12***"); // Field width was set to 5 with the '*' filler and left adjustment

    /*`It needs to be remembered that `boost::cnv::stream` converter uses `std::stream` as its underlying
       conversion engine. Consequently, formatting-related behavior are driven by the `std::stream`. Namely,
       after every operation is performed, the ['default field width is restored]. The values of
       the fill character and the adjustment remain unchanged until they are modified explicitly.
     */

    // The fill and adjustment remain '*' and 'left'.
    string s11 = convert<string>(12, cnv(arg::width = 4)).value();
    string s12 = convert<string>(12, cnv(arg::width = 5)
                                        (arg::fill = ' ')
                                        (arg::adjustment = cnv::adjustment::right)).value();

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

static
void
test_locale()
{
    //[stream_locale_example1
    boost::cnv::cstream cnv;
    std::locale  rus_locale;
    std::locale  eng_locale;

    char const* eng_locale_name = test::cnv::is_msc ? "?MS-US?LOCALE"
                                : test::cnv::is_gcc ? "en_US.UTF-8"
                                : "";
    char const* rus_locale_name = test::cnv::is_msc ? "Russian_Russia.1251"
                                : test::cnv::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::cnv::is_msc ? "1,235e-002"
                                : test::cnv::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::cnv::is_msc ? "1.235e-002"
                                : test::cnv::is_gcc ? "1.235e-02" : "";
    char const*      double_s01 = test::cnv::is_msc ? "1.2345E-002"
                                : test::cnv::is_gcc ? "1.2345E-02"
                                : "";

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

void
test::cnv::stream_converter()
{
    test_numbase();
    test_boolalpha();
    test_skipws();
    test_width();
    test_manipulators();
    test_locale();

    boost::cnv::cstream ccnv; // std::stringstream-based char converter
    boost::cnv::wstream wcnv; // std::stringstream-based wchar_t converter

    string const    not_int_str = "not an int";
    string const        std_str = "-11";
    char const* const     c_str = "-12";
    wstring const      wstd_str = L"-13";
    wchar_t const* const wc_str = L"-14";
    char const      array_str[] = "-15";

    BOOST_TEST("377" == boost::convert<std::string>(255, ccnv(arg::base = boost::cnv::base::oct)).value());
    BOOST_TEST( "ff" == boost::convert<std::string>(255, ccnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("255" == boost::convert<std::string>(255, ccnv(arg::base = boost::cnv::base::dec)).value());

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s01 = boost::convert< string>(-1, ccnv).value_or(std_str);
    string                    s02 = boost::convert< string>(-2, ccnv).value_or(c_str);
    wstring                   s03 = boost::convert<wstring>(-3, wcnv).value_or(wstd_str);
    wstring                   s04 = boost::convert<wstring>(-4, wcnv).value_or(wc_str);
    string                    s05 = boost::convert< string>(-5, ccnv).value_or(array_str);
    boost::optional< string> rs01 = boost::convert< string>(-1, ccnv);
    boost::optional< string> rs02 = boost::convert< string>(-2, ccnv);
    boost::optional<wstring> rs03 = boost::convert<wstring>(-3, wcnv);
    boost::optional<wstring> rs04 = boost::convert<wstring>(-4, wcnv);
    boost::optional< string> rs05 = boost::convert< string>(-5, ccnv);

    BOOST_TEST(s01 ==  "-1"); BOOST_TEST(rs01 && rs01.value() ==  "-1");
    BOOST_TEST(s02 ==  "-2"); BOOST_TEST(rs02 && rs02.value() ==  "-2");
    BOOST_TEST(s03 == L"-3"); BOOST_TEST(rs03 && rs03.value() == L"-3");
    BOOST_TEST(s04 == L"-4"); BOOST_TEST(rs04 && rs04.value() == L"-4");
    BOOST_TEST(s05 ==  "-5"); BOOST_TEST(rs05 && rs05.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s11 = boost::convert< string>(-1, ccnv).value();
    wstring                   s12 = boost::convert<wstring>(-2, wcnv).value();
    boost::optional< string> rs11 = boost::convert< string>(-1, ccnv);
    boost::optional<wstring> rs12 = boost::convert<wstring>(-2, wcnv);

    BOOST_TEST( s11 ==  "-1");
    BOOST_TEST( s12 == L"-2");
    BOOST_TEST(rs11 && rs11.value() ==  "-1");
    BOOST_TEST(rs12 && rs12.value() == L"-2");
}
