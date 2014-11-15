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

#define VALUE_TEST(CONVERT, RESULT) BOOST_TEST(CONVERT && CONVERT.value() == RESULT)

static
void
test_dbl_to_str()
{
    boost::cnv::cstream cnv;

    cnv(std::fixed);

    VALUE_TEST(convert<string>( 99.999, cnv(arg::precision = 2)), "100.00");
    VALUE_TEST(convert<string>( 99.949, cnv(arg::precision = 2)),  "99.95");
    VALUE_TEST(convert<string>(-99.949, cnv(arg::precision = 2)), "-99.95");
    VALUE_TEST(convert<string>( 99.949, cnv(arg::precision = 1)),   "99.9");
    VALUE_TEST(convert<string>(  0.999, cnv(arg::precision = 2)),   "1.00");
    VALUE_TEST(convert<string>( -0.999, cnv(arg::precision = 2)),  "-1.00");
    VALUE_TEST(convert<string>(  0.949, cnv(arg::precision = 2)),   "0.95");
    VALUE_TEST(convert<string>( -0.949, cnv(arg::precision = 2)),  "-0.95");
    VALUE_TEST(convert<string>(  1.949, cnv(arg::precision = 1)),    "1.9");
    VALUE_TEST(convert<string>( -1.949, cnv(arg::precision = 1)),   "-1.9");
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

    VALUE_TEST(convert<int>( "11", ccnv(std::hex)), 17); // 11(16) = 17(10)
    VALUE_TEST(convert<int>( "11", ccnv(std::oct)),  9); // 11(8)  = 9(10)
    VALUE_TEST(convert<int>( "11", ccnv(std::dec)), 11);

    VALUE_TEST(convert<string>( 18, ccnv(std::hex)), "12"); // 18(10) = 12(16)
    VALUE_TEST(convert<string>( 10, ccnv(std::oct)), "12"); // 10(10) = 12(8)
    VALUE_TEST(convert<string>( 12, ccnv(std::dec)), "12");
    VALUE_TEST(convert<string>(255, ccnv(arg::base = boost::cnv::base::oct)), "377");
    VALUE_TEST(convert<string>(255, ccnv(arg::base = boost::cnv::base::hex)),  "ff");
    VALUE_TEST(convert<string>(255, ccnv(arg::base = boost::cnv::base::dec)), "255");

    ccnv(std::showbase);

    VALUE_TEST(convert<string>(18, ccnv(std::hex)), "0x12");
    VALUE_TEST(convert<string>(10, ccnv(std::oct)),  "012");

    ccnv(std::uppercase);

    VALUE_TEST(convert<string>(18, ccnv(std::hex)), "0X12");
    //]
    //[stream_numbase_example2
    VALUE_TEST(convert<int>("11", ccnv(arg::base = cnv::base::hex)), 17);
    VALUE_TEST(convert<int>("11", ccnv(arg::base = cnv::base::oct)),  9);
    VALUE_TEST(convert<int>("11", ccnv(arg::base = cnv::base::dec)), 11);
    //]
    //[wide_stream_numeric_base
    boost::cnv::wstream wcnv;

    VALUE_TEST(convert<int>(L"11", wcnv(std::hex)), 17); // 11(16) = 17(10)
    VALUE_TEST(convert<int>(L"11", wcnv(std::oct)),  9); // 11(8)  = 9(10)
    VALUE_TEST(convert<int>(L"11", wcnv(std::dec)), 11);

    VALUE_TEST(convert<wstring>(254, wcnv(arg::base = cnv::base::dec)), L"254");
    VALUE_TEST(convert<wstring>(254, wcnv(arg::base = cnv::base::hex)),  L"fe");
    VALUE_TEST(convert<wstring>(254, wcnv(arg::base = cnv::base::oct)), L"376");
    //]
}

static
void
test_boolalpha()
{
    boost::cnv::cstream cnv;
    //[stream_boolalpha_example
    VALUE_TEST(convert<string>( true, cnv(std::boolalpha)),  "true");
    VALUE_TEST(convert<string>(false, cnv(std::boolalpha)), "false");

    VALUE_TEST(convert<bool>( "true", cnv(std::boolalpha)),  true);
    VALUE_TEST(convert<bool>("false", cnv(std::boolalpha)), false);

    VALUE_TEST(convert<string>( true, cnv(std::noboolalpha)), "1");
    VALUE_TEST(convert<string>(false, cnv(std::noboolalpha)), "0");

    VALUE_TEST(convert<bool>("1", cnv(std::noboolalpha)),  true);
    VALUE_TEST(convert<bool>("0", cnv(std::noboolalpha)), false);
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

    VALUE_TEST(convert<int>(cstr_good, ccnv),    123);
    VALUE_TEST(convert<string>("  123", ccnv), "123");

    BOOST_TEST(!convert<int>(cstr_bad, ccnv));

    ccnv(std::noskipws); // Do not ignore leading whitespaces

    // All conversions fail.
    BOOST_TEST(!convert<int>(cstr_good, ccnv));
    BOOST_TEST(!convert<int>( cstr_bad, ccnv));
    //]
    //[stream_skipws2_example
    ccnv(arg::skipws = true); // Skip leading whitespaces

    VALUE_TEST(convert<   int>(cstr_good, ccnv),   123);
    VALUE_TEST(convert<string>(cstr_good, ccnv), "123");
    //]
    //[wide_stream_skipws
    boost::cnv::wstream       wcnv;
    wchar_t const* const wstr_good = L"  123";
    wchar_t const* const  wstr_bad = L"  123 ";

    wcnv(std::skipws);

    VALUE_TEST(convert<int>(wstr_good, wcnv), 123);
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

    boost::optional<string> s01 = convert<string>(12, cnv(std::setw(4)));
    boost::optional<string> s02 = convert<string>(12, cnv(std::setw(5))(std::setfill('*')));
    boost::optional<string> s03 = convert<string>(12, cnv(std::setw(5))(std::setfill('*'))(std::left));

    BOOST_TEST(s01 && s01.value() == "  12");  // Field width = 4.
    BOOST_TEST(s02 && s02.value() == "***12"); // Field width = 5, filler = '*'.
    BOOST_TEST(s03 && s03.value() == "12***"); // Field width = 5, filler = '*', left adjustment

    /*`It needs to be remembered that `boost::cnv::stream` converter uses `std::stream` as its underlying
       conversion engine. Consequently, formatting-related behavior are driven by the `std::stream`. Namely,
       after every operation is performed, the ['default field width is restored]. The values of
       the fill character and the adjustment remain unchanged until they are modified explicitly.
     */

    // The fill and adjustment remain '*' and 'left'.
    boost::optional<string> s11 = convert<string>(12, cnv(arg::width = 4));
    boost::optional<string> s12 = convert<string>(12, cnv(arg::width = 5)
                                        (arg::fill = ' ')
                                        (arg::adjust = cnv::adjust::right));

    BOOST_TEST(s11 && s11.value() == "12**");  // Field width was set to 4.
    BOOST_TEST(s12 && s12.value() == "   12"); // Field width was set to 5 with the ' ' filler.
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

    VALUE_TEST(boost::convert<string>(255, ccnv), "377");
    VALUE_TEST(boost::convert<string>( 15, ccnv),  "17");

    ccnv(std::showbase);

    VALUE_TEST(boost::convert<string>(255, ccnv), "0377");
    VALUE_TEST(boost::convert<string>( 15, ccnv),  "017");

    ccnv(std::uppercase)(std::hex);

    VALUE_TEST(boost::convert<string>(255, ccnv), "0XFF");
    VALUE_TEST(boost::convert<string>( 15, ccnv),  "0XF");

    ccnv(std::noshowbase)(std::nouppercase)(std::oct);

    VALUE_TEST(boost::convert<string>(255, ccnv), "377");
    VALUE_TEST(boost::convert<string>( 15, ccnv),  "17");

    ccnv(std::showbase)(arg::uppercase = true)(arg::base = cnv::base::hex);

    VALUE_TEST(boost::convert<string>(255, ccnv), "0XFF");
    VALUE_TEST(boost::convert<string>( 15, ccnv),  "0XF");
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

    double double_v01 = convert<double>(double_s01, cnv).value_or(0);
    string double_s02 = convert<string>(double_v01, cnv).value_or("bad");

    BOOST_TEST(double_s01 == double_s02);

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Bad locale %s.\n", rus_locale_name); exit(1); }

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Bad locale %s.\n", eng_locale_name); exit(1); }

//  cnv(std::setprecision(3))(std::nouppercase);
    cnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = convert<string>(double_v01, cnv(rus_locale)).value_or("bad double_rus");
    string double_eng = convert<string>(double_v01, cnv(eng_locale)).value_or("bad double_eng");

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

    double double_v01 = convert<double>(double_s01, cnv).value_or(0);
    string double_s02 = convert<string>(double_v01, cnv).value_or("bad");

    BOOST_TEST(double_s01 == double_s02);

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Bad locale %s. Ignored.\n", eng_locale_name); eng_ignore = true; }

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Bad locale %s. Ignored.\n", rus_locale_name); rus_ignore = true; }

//  cnv(std::setprecision(3))(std::nouppercase);
    cnv(arg::precision = 3)(arg::uppercase = false);

    if (!eng_ignore) VALUE_TEST(convert<string>(double_v01, cnv(eng_locale)), eng_expected);
    if (!rus_ignore) VALUE_TEST(convert<string>(double_v01, cnv(rus_locale)), rus_expected);
}

static
void
test_user_str()
{
    //[stream_my_string
    boost::cnv::cstream cnv;
    my_string        my_str("123");

    cnv(std::setprecision(2))(std::fixed);

    VALUE_TEST(convert<int>(my_str, cnv), 123);

    VALUE_TEST(convert<my_string>( 99.999, cnv), "100.00");
    VALUE_TEST(convert<my_string>( 99.949, cnv),  "99.95");
    VALUE_TEST(convert<my_string>(-99.949, cnv), "-99.95");
    //]
}

int
main(int argc, char const* argv[])
{
	try
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
	catch(boost::bad_optional_access const&)
	{
	    BOOST_TEST(!"Caught boost::bad_optional_access exception");
	}
	catch(...)
	{
	    BOOST_TEST(!"Caught an unknown exception");
	}
    return boost::report_errors();
}
