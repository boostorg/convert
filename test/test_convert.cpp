// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#if !defined(_MSC_VER)
#include "./sfinae.cpp"
#include "./algorithms.cpp"
#include "./performance.cpp"
#include "./encryption.cpp"
#include "./callable.cpp"
#endif

using std::string;
using std::wstring;
using boost::convert;

template<typename Converter>
void
test::type_to_string(Converter const& cnv)
{
    BOOST_TEST("255" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::string_to_type(Converter const& cnv)
{
    BOOST_TEST( 255 == boost::convert<int>::from("255", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 999 == boost::convert<int>::from( "FF", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 255 == boost::convert<int>::from( "FF", cnv(arg::base = cnv::base::hex)).value_or(999));
    BOOST_TEST( 173 == boost::convert<int>::from("255", cnv(arg::base = cnv::base::oct)).value_or(999));
    BOOST_TEST( 999 != boost::convert<double>::from("1.23", cnv).value_or(999));
}

int
main(int argc, char const* argv[])
{
	test::sfinae();
    test::string_to_type(boost::strtol_converter()); 
    test::string_to_type(boost::printf_converter());
	test::type_to_string(boost::printf_converter());
	test::algorithms();
	test::performance();
    test::encryption();
    test::callables();

    string const           not_int_str = "not an int";
    string const               std_str = "-11";
    char const* const            c_str = "-12";
    wstring const             wstd_str = L"-13";
    wchar_t const* const        wc_str = L"-14";
    char const             array_str[] = "-15";
    char const* const    container_str = "-16";
    std::vector<char> const vector_str (container_str, container_str + strlen(container_str));
    std::list<char> const     list_str (container_str, container_str + strlen(container_str));

    boost::lexical_cast_converter  lcnv; // lexical_cast-based converter
    boost::cstringstream_converter ccnv; // stringstream-based char converter
	boost::wstringstream_converter wcnv; // stringstream-based wchar_t converter
		
    ////////////////////////////////////////////////////////////////////////////
    // Testing lexical_cast-based converter.
    ////////////////////////////////////////////////////////////////////////////

    int lc00 = convert<int>::from(not_int_str, lcnv).value_or(-1);
    int lc01 = convert<int>::from(std_str,     lcnv).value_or(-1);
    int lc02 = convert<int>::from(c_str,       lcnv).value_or(-1);

    BOOST_TEST(lc00 ==  -1); // Failed conversion. No throw
    BOOST_TEST(lc01 == -11);
    BOOST_TEST(lc02 == -12);

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int a000 = convert<int>::from(not_int_str, ccnv).value_or(-1);
    int a001 = convert<int>::from(std_str,     ccnv).value_or(-1);
    int a002 = convert<int>::from(c_str,       ccnv).value_or(-1);
    int a003 = convert<int>::from(wstd_str,    wcnv).value_or(-1);
    int a004 = convert<int>::from(wc_str,      wcnv).value_or(-1);
    int a005 = convert<int>::from(array_str,   ccnv).value_or(-1);

    BOOST_TEST(a000 ==  -1); // Failed conversion
    BOOST_TEST(a001 == -11);
    BOOST_TEST(a002 == -12);
    BOOST_TEST(a003 == -13);
    BOOST_TEST(a004 == -14);
    BOOST_TEST(a005 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to convert<TypeOut>::result.
    // convert<>::result exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r000 = convert<int>::from(not_int_str, ccnv);
    convert<int>::result r001 = convert<int>::from(std_str,     ccnv);
    convert<int>::result r002 = convert<int>::from(c_str,       ccnv);
    convert<int>::result r003 = convert<int>::from(wstd_str,    wcnv);
    convert<int>::result r004 = convert<int>::from(wc_str,      wcnv);
    convert<int>::result r005 = convert<int>::from(array_str,   ccnv);

    BOOST_TEST(!r000); // Failed conversion
    BOOST_TEST( r001 && r001.value() == -11);
    BOOST_TEST( r002 && r002.value() == -12);
    BOOST_TEST( r003 && r003.value() == -13);
    BOOST_TEST( r004 && r004.value() == -14);
    BOOST_TEST( r005 && r005.value() == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        convert<int>::from(not_int_str, ccnv).value();
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    int a021 = convert<int>::from(std_str,   ccnv).value();
    int a022 = convert<int>::from(c_str,     ccnv).value();
    int a023 = convert<int>::from(wstd_str,  wcnv).value();
    int a024 = convert<int>::from(wc_str,    wcnv).value();
    int a025 = convert<int>::from(array_str, ccnv).value();

    BOOST_TEST(a021 == -11);
    BOOST_TEST(a022 == -12);
    BOOST_TEST(a023 == -13);
    BOOST_TEST(a024 == -14);
    BOOST_TEST(a025 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing convert<>::result interface.
    // convert<>::result exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r010 = convert<int>::from(not_int_str, ccnv);
    convert<int>::result r011 = convert<int>::from(std_str, ccnv);
    convert<int>::result r012 = convert<int>::from(c_str, ccnv);
    convert<int>::result r013 = convert<int>::from(wstd_str, wcnv);
    convert<int>::result r014 = convert<int>::from(wc_str, wcnv);
    convert<int>::result r015 = convert<int>::from(array_str, ccnv);

    BOOST_TEST(!r010); // Failed conversion
    BOOST_TEST( r011 && r011.value() == -11);
    BOOST_TEST( r012 && r012.value() == -12);
    BOOST_TEST( r013 && r013.value() == -13);
    BOOST_TEST( r014 && r014.value() == -14);
    BOOST_TEST( r015 && r015.value() == -15);

    try
    {
        r010.value(); // Throws on an attempt to retrieve the value.
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s001 = convert< string>::from(-5, ccnv).value_or(std_str);
    string                    s002 = convert< string>::from(-5, ccnv).value_or(c_str);
    wstring                   s003 = convert<wstring>::from(-5, wcnv).value_or(wstd_str);
    wstring                   s004 = convert<wstring>::from(-5, wcnv).value_or(wc_str);
    string                    s005 = convert< string>::from(-5, ccnv).value_or(array_str);
    convert< string>::result rs001 = convert< string>::from(-5, ccnv);
    convert< string>::result rs002 = convert< string>::from(-5, ccnv);
    convert<wstring>::result rs003 = convert<wstring>::from(-5, wcnv);
    convert<wstring>::result rs004 = convert<wstring>::from(-5, wcnv);
    convert< string>::result rs005 = convert< string>::from(-5, ccnv);

    BOOST_TEST(s001 ==  "-5"); BOOST_TEST(rs001 && rs001.value() ==  "-5");
    BOOST_TEST(s002 ==  "-5"); BOOST_TEST(rs002 && rs002.value() ==  "-5");
    BOOST_TEST(s003 == L"-5"); BOOST_TEST(rs003 && rs003.value() == L"-5");
    BOOST_TEST(s004 == L"-5"); BOOST_TEST(rs004 && rs004.value() == L"-5");
    BOOST_TEST(s005 ==  "-5"); BOOST_TEST(rs005 && rs005.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s010 = convert< string>::from(-5, ccnv).value();
    wstring                   s011 = convert<wstring>::from(-5, wcnv).value();
    convert< string>::result rs010 = convert< string>::from(-5, ccnv);
    convert<wstring>::result rs011 = convert<wstring>::from(-5, wcnv);

    BOOST_TEST( s010 ==  "-5");
    BOOST_TEST( s011 == L"-5");
    BOOST_TEST(rs010 && rs010.value() ==  "-5");
    BOOST_TEST(rs011 && rs011.value() == L"-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversions of a user-defined type
    ////////////////////////////////////////////////////////////////////////////

    direction const            up_dir1 = direction::up;
    direction const            dn_dir1 = direction::dn;
    string                 up_dir0_str = convert<string>::from<direction>(direction::up, ccnv).value();
    string                 dn_dir0_str = convert<string>::from<direction>(direction::dn, ccnv).value();
    string                 up_dir1_str = convert<string>::from(up_dir1, ccnv).value();
    string                 dn_dir1_str = convert<string>::from(dn_dir1, ccnv).value();
    direction                  up_dir2 = convert<direction>::from(up_dir1_str, ccnv).value();
    direction                  dn_dir2 = convert<direction>::from(dn_dir1_str, ccnv).value();
    direction                  up_dir3 = convert<direction>::from(up_dir1_str, ccnv).value();
    direction                  dn_dir3 = convert<direction>::from(dn_dir1_str, ccnv).value();
    direction                  dn_dir4 = convert<direction>::from("junk", ccnv).value_or(direction::dn);
    convert<direction>::result up_dir4 = convert<direction>::from("junk", ccnv);

    BOOST_TEST(up_dir0_str == "up");
    BOOST_TEST(dn_dir0_str == "dn");
    BOOST_TEST(up_dir1_str == "up");
    BOOST_TEST(dn_dir1_str == "dn");
    BOOST_TEST(up_dir2 == up_dir1);
    BOOST_TEST(dn_dir2 == dn_dir1);
    BOOST_TEST(up_dir3 == direction::up);
    BOOST_TEST(dn_dir3 == direction::dn);
    BOOST_TEST(dn_dir4 == direction::dn);
    BOOST_TEST(!up_dir4); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing formatters/manipulators
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        boost::lexical_cast<int>("FF");
        BOOST_TEST(!"We should not be here");
    }
    catch (...)
    {
    }

    int hex_v01 = convert<int>::from("FF", ccnv(std::hex)).value_or(0);
    int hex_v02 = convert<int>::from(L"F", wcnv(std::hex)).value_or(0);
    int hex_v03 = convert<int>::from("FF", ccnv(std::dec)).value_or(-5);
    int hex_v04 = convert<int>::from(L"F", wcnv(std::dec)).value_or(-5);

    BOOST_TEST(hex_v01 == 255); // "FF"
    BOOST_TEST(hex_v02 ==  15); // L"F"
    BOOST_TEST(hex_v03 ==  -5);
    BOOST_TEST(hex_v04 ==  -5);

    ccnv(std::showbase)(std::uppercase)(std::hex);

    BOOST_TEST(convert<string>::from(255, ccnv).value() == "0XFF");
    BOOST_TEST(convert<string>::from( 15, ccnv).value() ==  "0XF");

    char const*  double_s01 = test::is_msc ? "1.2345E-002"
                            : test::is_gcc ? "1.2345E-02"
                            : "";
//  ccnv(std::setprecision(4))(std::uppercase)(std::scientific);
    ccnv(arg::precision = 4)
        (arg::uppercase = true)
        (arg::notation = cnv::notation::scientific);

    double double_v01 = convert<double>::from(double_s01, ccnv).value();
    string double_s02 = convert<string>::from(double_v01, ccnv).value();

    BOOST_TEST(double_s01 == double_s02);

    ////////////////////////////////////////////////////////////////////////////
    // Testing locale
    ////////////////////////////////////////////////////////////////////////////

    char const* rus_locale_name = test::is_msc ? "Russian_Russia.1251"
                                : test::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::is_msc ? "1,235e-002" : test::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::is_msc ? "1.235e-002" : test::is_gcc ? "1.235e-02" : "";
    std::locale      rus_locale;
    std::locale      eng_locale;

    try
    {
        rus_locale = std::locale(rus_locale_name);
        eng_locale = std::locale("");
    }
    catch (...)
    {
        printf("Test failed: bad locale %s.\n", rus_locale_name);
        exit(1);
    }

//  ccnv(std::setprecision(3))(std::nouppercase);
    ccnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = convert<string>::from(double_v01, ccnv(rus_locale)).value();
    string double_eng = convert<string>::from(double_v01, ccnv(eng_locale)).value();
    printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_rus.c_str());
    printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_eng.c_str());

    BOOST_TEST(double_rus == rus_expected);
    BOOST_TEST(double_eng == eng_expected);

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

//    convert<bool>::result bool_res1 = convert<bool>::from("1",     false, ccnv);
//    convert<bool>::result bool_res2 = convert<bool>::from("true",  false, ccnv);
//    convert<bool>::result bool_res3 = convert<bool>::from("yes",   false, ccnv);
//    convert<bool>::result bool_res4 = convert<bool>::from(L"1",    false, wcnv);
//    convert<bool>::result bool_res5 = convert<bool>::from(L"true", false, wcnv);
//    convert<bool>::result bool_res6 = convert<bool>::from(L"yes",  false, wcnv);
//    convert<bool>::result bool_res7 = convert<bool>::from("junk",   true, ccnv);
//
//    BOOST_TEST( bool_res1 && bool_res1.value() == true);
//    BOOST_TEST( bool_res2 && bool_res2.value() == true);
//    BOOST_TEST( bool_res3 && bool_res3.value() == true);
//    BOOST_TEST( bool_res4 && bool_res4.value() == true);
//    BOOST_TEST( bool_res5 && bool_res5.value() == true);
//    BOOST_TEST( bool_res6 && bool_res6.value() == true);
//    BOOST_TEST(!bool_res7 && bool_res7.value() == true);
//
//    string bool_str1 = convert<string>::from(true,  ccnv).value_or("failed");
//    string bool_str2 = convert<string>::from(false, ccnv).value_or("failed");
//    string bool_str3 = convert<string>::from(1,     ccnv).value_or("failed");
//    string bool_str4 = convert<string>::from(0,     ccnv).value_or("failed");
//
//    BOOST_TEST(bool_str1 == "1");
//    BOOST_TEST(bool_str2 == "0");
//    BOOST_TEST(bool_str3 == "1");
//    BOOST_TEST(bool_str4 == "0");

    return boost::report_errors();
}
