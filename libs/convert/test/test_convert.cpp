// Boost.Convert library test and usage example
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost.xtra/boost/convert/api.hpp>
#include <boost.xtra/boost/convert/sstream_based_converter.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iomanip>
#include <vector>
#include <list>
#include <stdio.h>

using std::string;
using std::wstring;
using boost::array;
using boost::convert;
namespace cnv = boost::conversion;

std::basic_ios<char>&
my_cypher(std::basic_ios<char>& stream)
{
    std::streambuf* buf = stream.rdbuf();
    size_t       cypher = 'A' - '1';
    string         data;

    for (int c = 0; c = buf->sbumpc(), c != EOF; data += c);

    for (string::iterator it = data.begin(); it != data.end(); ++it)
        *it < 'A' ? buf->sputc(*it += cypher) : buf->sputc(*it -= cypher);

    return stream;
}

struct direction
{
    // Note: the class does NOT have the default constructor.

    enum value_type { up, dn };

    direction(value_type value) : value_(value) {}
    bool operator==(direction const& that) const { return value_ == that.value_; }

    friend std::istream& operator>>(std::istream& stream, direction& dir)
    {
        string str; stream >> str;

        /**/ if (str == "up") dir.value_ = up;
        else if (str == "dn") dir.value_ = dn;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, direction const& dir)
    {
        return (stream << (dir.value_ == up ? "up" : "dn"));
    }

    private: value_type value_;
};

int
main(int argc, char const* argv[])
{
    ////////////////////////////////////////////////////////////////////////////
    // Test string SFINAE.
    ////////////////////////////////////////////////////////////////////////////

    bool q01 = boost::convert_detail::is_any_string<std::string>::value;
    bool q02 = boost::convert_detail::is_any_string<char const*>::value;
    bool q03 = boost::convert_detail::is_any_string<std::wstring>::value;
    bool q04 = boost::convert_detail::is_any_string<wchar_t const*>::value;
    bool q05 = boost::convert_detail::is_any_string<char []>::value;
    bool q06 = boost::convert_detail::is_any_string<wchar_t []>::value;
    bool q07 = boost::convert_detail::is_any_string<std::vector<char> >::value;
    bool q08 = boost::convert_detail::is_any_string<std::list<wchar_t> >::value;
    bool q98 = boost::convert_detail::is_any_string<int>::value;
    bool q99 = boost::convert_detail::is_any_string<direction>::value;

    BOOST_ASSERT( q01);
    BOOST_ASSERT( q02);
    BOOST_ASSERT( q03);
    BOOST_ASSERT( q04);
    BOOST_ASSERT( q05);
    BOOST_ASSERT( q06);
    BOOST_ASSERT(!q07); // Support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q08); // Support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q98);
    BOOST_ASSERT(!q99);

    string const           not_int_str = "not an int";
    string const               std_str = "-11";
    char const* const            c_str = "-12";
    wstring const             wstd_str = L"-13";
    wchar_t const* const        wc_str = L"-14";
    char const             array_str[] = "-15";
    char const* const    container_str = "-16";
    std::vector<char> const vector_str (container_str, container_str + strlen(container_str));
    std::list<char> const     list_str (container_str, container_str + strlen(container_str));

	boost::cstringstream_based_converter ccnv; // char converter
	boost::wstringstream_based_converter wcnv; // wchar_t converter
		
	////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int a000 = convert<int>::from(not_int_str,   -1, ccnv);
    int a001 = convert<int>::from(std_str,       -1, ccnv);
    int a002 = convert<int>::from(c_str,         -1, ccnv);
    int a003 = convert<int>::from(wstd_str,      -1, wcnv);
    int a004 = convert<int>::from(wc_str,        -1, wcnv);
    int a005 = convert<int>::from(array_str,     -1, ccnv);
//  int a006 = convert<int>::from(vector_str,    -1, ccnv); support withdrawn
//  int a007 = convert<int>::from(list_str,      -1, ccnv); support withdrawn

    BOOST_ASSERT(a000 ==  -1); // Failed conversion
    BOOST_ASSERT(a001 == -11);
    BOOST_ASSERT(a002 == -12);
    BOOST_ASSERT(a003 == -13);
    BOOST_ASSERT(a004 == -14);
    BOOST_ASSERT(a005 == -15);
//    BOOST_ASSERT(a006 == -16);
//    BOOST_ASSERT(a007 == -17);

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to convert<TypeOut>::result.
    // convert<>::result exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r000 = convert<int>::from(not_int_str,   -1, ccnv);
    convert<int>::result r001 = convert<int>::from(std_str,       -1, ccnv);
    convert<int>::result r002 = convert<int>::from(c_str,         -1, ccnv);
    convert<int>::result r003 = convert<int>::from(wstd_str,      -1, wcnv);
    convert<int>::result r004 = convert<int>::from(wc_str,        -1, wcnv);
    convert<int>::result r005 = convert<int>::from(array_str,     -1, ccnv);

    BOOST_ASSERT(!r000 && r000.value() ==  -1); // Failed conversion
    BOOST_ASSERT( r001 && r001.value() == -11);
    BOOST_ASSERT( r002 && r002.value() == -12);
    BOOST_ASSERT( r003 && r003.value() == -13);
    BOOST_ASSERT( r004 && r004.value() == -14);
    BOOST_ASSERT( r005 && r005.value() == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        convert<int>::from(not_int_str, ccnv).value();
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    int a021 = convert<int>::from(std_str, ccnv);
    int a022 = convert<int>::from(c_str, ccnv);
    int a023 = convert<int>::from(wstd_str, wcnv);
    int a024 = convert<int>::from(wc_str, wcnv);
    int a025 = convert<int>::from(array_str, ccnv);

    BOOST_ASSERT(a021 == -11);
    BOOST_ASSERT(a022 == -12);
    BOOST_ASSERT(a023 == -13);
    BOOST_ASSERT(a024 == -14);
    BOOST_ASSERT(a025 == -15);

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

    BOOST_ASSERT(!r010); // Failed conversion
    BOOST_ASSERT( r011 && r011.value() == -11);
    BOOST_ASSERT( r012 && r012.value() == -12);
    BOOST_ASSERT( r013 && r013.value() == -13);
    BOOST_ASSERT( r014 && r014.value() == -14);
    BOOST_ASSERT( r015 && r015.value() == -15);

    try
    {
        r010.value(); // Throws on an attempt to retrieve the value.
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s001 = convert< string>::from(-5, std_str, ccnv);
    string                    s002 = convert< string>::from(-5, c_str, ccnv);
    wstring                   s003 = convert<wstring>::from(-5, wstd_str, wcnv);
    wstring                   s004 = convert<wstring>::from(-5, wc_str, wcnv);
    string                    s005 = convert< string>::from(-5, array_str, ccnv);
    convert< string>::result rs001 = convert< string>::from(-5, std_str, ccnv);
    convert< string>::result rs002 = convert< string>::from(-5, c_str, ccnv);
    convert<wstring>::result rs003 = convert<wstring>::from(-5, wstd_str, wcnv);
    convert<wstring>::result rs004 = convert<wstring>::from(-5, wc_str, wcnv);
    convert< string>::result rs005 = convert< string>::from(-5, array_str, ccnv);

    BOOST_ASSERT(s001 ==  "-5"); BOOST_ASSERT(rs001 && rs001.value() ==  "-5");
    BOOST_ASSERT(s002 ==  "-5"); BOOST_ASSERT(rs002 && rs002.value() ==  "-5");
    BOOST_ASSERT(s003 == L"-5"); BOOST_ASSERT(rs003 && rs003.value() == L"-5");
    BOOST_ASSERT(s004 == L"-5"); BOOST_ASSERT(rs004 && rs004.value() == L"-5");
    BOOST_ASSERT(s005 ==  "-5"); BOOST_ASSERT(rs005 && rs005.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s010 = convert< string>::from(-5, ccnv);
    wstring                   s011 = convert<wstring>::from(-5, wcnv);
    convert< string>::result rs010 = convert< string>::from(-5, ccnv);
    convert<wstring>::result rs011 = convert<wstring>::from(-5, wcnv);

    BOOST_ASSERT( s010 ==  "-5");
    BOOST_ASSERT( s011 == L"-5");
    BOOST_ASSERT(rs010 && rs010.value() ==  "-5");
    BOOST_ASSERT(rs011 && rs011.value() == L"-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversions of a user-defined type
    ////////////////////////////////////////////////////////////////////////////

    direction  up_dir1 = direction::up;
    direction  dn_dir1 = direction::dn;
    string up_dir1_str = convert<string>::from(up_dir1, ccnv);
    string dn_dir1_str = convert<string>::from(dn_dir1, ccnv);
    direction  up_dir2 = convert<direction>::from(up_dir1_str, dn_dir1, ccnv);
    direction  dn_dir2 = convert<direction>::from(dn_dir1_str, up_dir1, ccnv);
    direction  up_dir3 = convert<direction>::from(up_dir1_str, direction::dn, ccnv);
    direction  dn_dir3 = convert<direction>::from(dn_dir1_str, direction::up, ccnv);
//  direction  up_dir9 = convert<direction>::from(up_dir1_str, ccnv); // Doesn't compile. No def.ctor
//  direction  dn_dir9 = convert<direction>::from(dn_dir1_str, ccnv); // Doesn't compile. No def.ctor

    convert<direction>::result up_dir4 = convert<direction>::from("junk", dn_dir1, ccnv);

    BOOST_ASSERT(up_dir1_str == "up");
    BOOST_ASSERT(dn_dir1_str == "dn");
    BOOST_ASSERT(up_dir2 == up_dir1);
    BOOST_ASSERT(dn_dir2 == dn_dir1);
    BOOST_ASSERT(up_dir3 == direction::up);
    BOOST_ASSERT(dn_dir3 == direction::dn);
    BOOST_ASSERT(!up_dir4 && up_dir4.value() == direction::dn); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing explicitly specified throwing behavior.
    ////////////////////////////////////////////////////////////////////////////
    try
    {
        convert<direction>::from("junk", direction::up, ccnv)(cnv::parameter::throw_on_failure = true).value();
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }

    ////////////////////////////////////////////////////////////////////////////
    // Testing formatters/manipulators
    ////////////////////////////////////////////////////////////////////////////

    string         hex1_str = "0xFF"; // 255
    wchar_t const* hex2_str = L"0xF"; // 15
#if defined(_MSC_VER)
    char const*  double_s01 = "1.2345E-002"; // Windows
#elif defined(__GNUC__)
    char const*  double_s01 = "1.2345E-02"; // Linux
#endif
    int hex_v01 = convert<int>::from(hex1_str, 0, ccnv(std::hex));
    int hex_v02 = convert<int>::from(hex2_str, 0, wcnv(std::hex));

    BOOST_ASSERT(hex_v01 == 255);
    BOOST_ASSERT(hex_v02 ==  15);

    ccnv(std::showbase)(std::uppercase)(std::hex);

    string hex_s01 = convert<string>::from(hex_v01, ccnv);
    string hex_s02 = convert<string>::from(hex_v02, ccnv);

    BOOST_ASSERT(hex_s01 == "0XFF");
    BOOST_ASSERT(hex_s02 ==  "0XF");

    ccnv(std::setprecision(4))(std::scientific);

    double double_v01 = convert<double>::from(double_s01, ccnv);
    string double_s02 = convert<string>::from(double_v01, ccnv);

    BOOST_ASSERT(double_s01 == double_s02);

    ////////////////////////////////////////////////////////////////////////////
    // Testing locale
    ////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
    char const* rus_locale_name = "Russian_Russia.1251";
    char const*    rus_expected = "1,2345E-002";
    char const*    eng_expected = "1.2345E-002";
#elif defined(__CYGWIN__)
#error What is the locale name on Cygwin?
#elif defined(__GNUC__)
    char const* rus_locale_name = "ru_RU.UTF-8";
    char const*    rus_expected = "1,2345e-02";
    char const*    eng_expected = "1.2345e-02";
#endif
    try
    {
        std::locale rus_locale (rus_locale_name);
        std::locale eng_locale ("");

        ccnv(std::setprecision(4))(std::scientific)(std::nouppercase);

        string double_rus = convert<string>::from(double_v01, ccnv(rus_locale)).value();
        string double_eng = convert<string>::from(double_v01, ccnv(eng_locale)).value();
//      printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_s02.c_str());
//      printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_s03.c_str());

        BOOST_ASSERT(double_rus == rus_expected);
        BOOST_ASSERT(double_eng == eng_expected);
    }
    catch (...)
    {
        printf("Test failed: bad locale %s.\n", rus_locale_name);
        exit(1);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Testing boost::convert with algorithms
    ////////////////////////////////////////////////////////////////////////////

    array<char const*, 5>   strings = {{ "0XF", "0X10", "0X11", "0X12", "not int" }};
    std::vector<int>       integers;
    std::vector<string> new_strings;

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion
    // No explicit fallback value parameter. i.e. throws on failure. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////
    try
    {
        std::transform(
            strings.begin(),
            strings.end(),
            std::back_inserter(integers),
            convert<int>::from<string>(ccnv >> std::hex));

        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    BOOST_ASSERT(integers[0] == 15);
    BOOST_ASSERT(integers[1] == 16);
    BOOST_ASSERT(integers[2] == 17);
    BOOST_ASSERT(integers[3] == 18);

    integers.clear();

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion
    // Explicit fallback parameter, i.e. no throwing. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////
    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(integers),
        convert<int>::from<string>(ccnv(std::hex))(cnv::parameter::fallback = -1));

    BOOST_ASSERT(integers[0] == 15);
    BOOST_ASSERT(integers[1] == 16);
    BOOST_ASSERT(integers[2] == 17);
    BOOST_ASSERT(integers[3] == 18);
    BOOST_ASSERT(integers[4] == -1); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // int-to-string conversion. No explicit fallback value.
    ////////////////////////////////////////////////////////////////////////////
    std::transform(
        integers.begin(),
        integers.end(),
        std::back_inserter(new_strings),
        convert<string>::from<int>(ccnv >> std::dec));

    for (size_t k = 0; k < new_strings.size(); ++k)
        printf("%d. %s\n", int(k), new_strings[k].c_str());

    BOOST_ASSERT(new_strings[0] == "15");
    BOOST_ASSERT(new_strings[1] == "16");
    BOOST_ASSERT(new_strings[2] == "17");
    BOOST_ASSERT(new_strings[3] == "18");
    BOOST_ASSERT(new_strings[4] == "-1");

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-string conversion
    ////////////////////////////////////////////////////////////////////////////

    //string  s31 = convert< string>::from(std_str, ccnv);
    //string  s32 = convert< string>::from(c_str, ccnv);
    //wstring s35 = convert<wstring>::from(wstd_str, ccnv);
    //wstring s36 = convert<wstring>::from(wc_str, ccnv);
    //string  s37 = convert< string>::from(array_str, ccnv);

    //BOOST_ASSERT(s31 ==  "-11");
    //BOOST_ASSERT(s32 ==  "-12");
    //BOOST_ASSERT(s35 == L"-15");
    //BOOST_ASSERT(s36 == L"-16");
    //BOOST_ASSERT(s37 ==  "-17");

    ////////////////////////////////////////////////////////////////////////////
    // Testing custom manipulators.
    // I do not really know how to write manipulators.
    // Wrote my_cypher just to demonstrate the idea.
    ////////////////////////////////////////////////////////////////////////////

    //string encrypted = convert<string>::from("ABC", ccnv >> my_cypher);
    //string decrypted = convert<string>::from(encrypted, ccnv >> my_cypher);

    //BOOST_ASSERT(encrypted == "123");
    //BOOST_ASSERT(decrypted == "ABC");

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

    convert<bool>::result bool_res1 = convert<bool>::from("1",     false, ccnv);
    convert<bool>::result bool_res2 = convert<bool>::from("true",  false, ccnv);
    convert<bool>::result bool_res3 = convert<bool>::from("yes",   false, ccnv);
    convert<bool>::result bool_res4 = convert<bool>::from(L"1",    false, wcnv);
    convert<bool>::result bool_res5 = convert<bool>::from(L"true", false, wcnv);
    convert<bool>::result bool_res6 = convert<bool>::from(L"yes",  false, wcnv);
    convert<bool>::result bool_res7 = convert<bool>::from("junk",   true, ccnv);

    //BOOST_ASSERT( bool_res1 && bool_res1.value() == true);
    //BOOST_ASSERT( bool_res2 && bool_res2.value() == true);
    //BOOST_ASSERT( bool_res3 && bool_res3.value() == true);
    //BOOST_ASSERT( bool_res4 && bool_res4.value() == true);
    //BOOST_ASSERT( bool_res5 && bool_res5.value() == true);
    //BOOST_ASSERT( bool_res6 && bool_res6.value() == true);
    //BOOST_ASSERT(!bool_res7 && bool_res7.value() == true);

    string bool_str1 = convert<string>::from(true,  "failed", ccnv);
    string bool_str2 = convert<string>::from(false, "failed", ccnv);
    string bool_str3 = convert<string>::from(1,     "failed", ccnv);
    string bool_str4 = convert<string>::from(0,     "failed", ccnv);

    //BOOST_ASSERT(bool_str1 == "1");
    //BOOST_ASSERT(bool_str2 == "0");
    //BOOST_ASSERT(bool_str3 == "1");
    //BOOST_ASSERT(bool_str4 == "0");

    printf("Test completed successfully.\n");
    return 0;
}
