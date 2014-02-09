// Boost.Convert library test and usage example
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost.xtra/convert.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <iomanip>
#include <vector>
#include <list>

using std::string;
using std::wstring;
using boost::array;
using boost::convert;
using namespace boost::conversion::parameter;

std::basic_ios<char>&
my_cypher(std::basic_ios<char>& stream)
{
    std::streambuf* buf = stream.rdbuf();
    size_t       cypher = 'A' - '1';
    string         data;

    for (int c = 0; c = buf->sbumpc(), c != EOF; data += c);

    for (string::iterator it = data.begin(); it != data.end(); ++it)
    {
        *it < 'A' ? buf->sputc(*it += cypher) : buf->sputc(*it -= cypher);
    }
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
    // Test that SFINAE recognises string containers in their various forms.
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
    BOOST_ASSERT(!q07); // Original support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q08); // Original support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q98);
    BOOST_ASSERT(!q99);

    string            not_int_str = "not an int";
    string                std_str = "-11";
    char const*             c_str = "-12";
    string const    const_std_str = "-13";
    char const* const const_c_str = "-14";
    wstring              wstd_str = L"-15";
    wchar_t const*         wc_str = L"-16";
    char              array_str[] = "-17";
    char const*     container_str = "-18";
    std::vector<char>  vector_str (container_str, container_str + strlen(container_str));
    std::list<char>      list_str (container_str, container_str + strlen(container_str));

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int a001 = convert<int>::from(std_str,       -1);
    int a002 = convert<int>::from(c_str,         -1);
    int a003 = convert<int>::from(const_std_str, -1);
    int a004 = convert<int>::from(const_c_str,   -1);
    int a005 = convert<int>::from(wstd_str,      -1);
    int a006 = convert<int>::from(wc_str,        -1);
    int a007 = convert<int>::from(array_str,     -1);
    int a008 = convert<int>::from(not_int_str,   -1);
//  int a000 = convert<int>::from(vector_str,    -1); support withdrawn
//  int a000 = convert<int>::from(list_str,      -1); support withdrawn

    BOOST_ASSERT(a001 == -11);
    BOOST_ASSERT(a002 == -12);
    BOOST_ASSERT(a003 == -13);
    BOOST_ASSERT(a004 == -14);
    BOOST_ASSERT(a005 == -15);
    BOOST_ASSERT(a006 == -16);
    BOOST_ASSERT(a007 == -17);
    BOOST_ASSERT(a008 ==  -1); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to convert<TypeOut>::result.
    // convert<>::result exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r001 = convert<int>::from(std_str,       -1);
    convert<int>::result r002 = convert<int>::from(c_str,         -1);
    convert<int>::result r003 = convert<int>::from(const_std_str, -1);
    convert<int>::result r004 = convert<int>::from(const_c_str,   -1);
    convert<int>::result r005 = convert<int>::from(wstd_str,      -1);
    convert<int>::result r006 = convert<int>::from(wc_str,        -1);
    convert<int>::result r007 = convert<int>::from(array_str,     -1);
    convert<int>::result r008 = convert<int>::from(not_int_str,   -1);

    BOOST_ASSERT( r001 && r001.value() == -11);
    BOOST_ASSERT( r002 && r002.value() == -12);
    BOOST_ASSERT( r003 && r003.value() == -13);
    BOOST_ASSERT( r004 && r004.value() == -14);
    BOOST_ASSERT( r005 && r005.value() == -15);
    BOOST_ASSERT( r006 && r006.value() == -16);
    BOOST_ASSERT( r007 && r007.value() == -17);
    BOOST_ASSERT(!r008 && r008.value() ==  -1); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
    ////////////////////////////////////////////////////////////////////////////

    int a021 = convert<int>::from(std_str);
    int a022 = convert<int>::from(c_str);
    int a023 = convert<int>::from(const_std_str);
    int a024 = convert<int>::from(const_c_str);
    int a025 = convert<int>::from(wstd_str);
    int a026 = convert<int>::from(wc_str);
    int a027 = convert<int>::from(array_str);

    BOOST_ASSERT(a021 == -11);
    BOOST_ASSERT(a022 == -12);
    BOOST_ASSERT(a023 == -13);
    BOOST_ASSERT(a024 == -14);
    BOOST_ASSERT(a025 == -15);
    BOOST_ASSERT(a026 == -16);
    BOOST_ASSERT(a027 == -17);

    try
    {
        int a027 = convert<int>::from(not_int_str);
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    ////////////////////////////////////////////////////////////////////////////
    // Testing convert<>::result interface.
    // convert<>::result exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r011 = convert<int>::from(std_str);
    convert<int>::result r012 = convert<int>::from(c_str);
    convert<int>::result r013 = convert<int>::from(const_std_str);
    convert<int>::result r014 = convert<int>::from(const_c_str);
    convert<int>::result r015 = convert<int>::from(wstd_str);
    convert<int>::result r016 = convert<int>::from(wc_str);
    convert<int>::result r017 = convert<int>::from(array_str);
    convert<int>::result r018 = convert<int>::from(not_int_str);

    BOOST_ASSERT( r011 && r011.value() == -11);
    BOOST_ASSERT( r012 && r012.value() == -12);
    BOOST_ASSERT( r013 && r013.value() == -13);
    BOOST_ASSERT( r014 && r014.value() == -14);
    BOOST_ASSERT( r015 && r015.value() == -15);
    BOOST_ASSERT( r016 && r016.value() == -16);
    BOOST_ASSERT( r017 && r017.value() == -17);
    BOOST_ASSERT(!r018); // Failed conversion

    try
    {
        int r018_value = r018.value(); // Throws on an attempt to retrieve the value.
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

    string                    s001 = convert< string>::from(-5, std_str);
    string                    s002 = convert< string>::from(-5, c_str);
    string                    s003 = convert< string>::from(-5, const_std_str);
    string                    s004 = convert< string>::from(-5, const_c_str);
    wstring                   s005 = convert<wstring>::from(-5, wstd_str);
    wstring                   s006 = convert<wstring>::from(-5, wc_str);
    string                    s007 = convert< string>::from(-5, array_str);
    string                    s008 = convert< string>::from(-5, vector_str);
    string                    s009 = convert< string>::from(-5, list_str);
    convert< string>::result rs001 = convert< string>::from(-5, std_str);
    convert< string>::result rs002 = convert< string>::from(-5, c_str);
    convert< string>::result rs003 = convert< string>::from(-5, const_std_str);
    convert< string>::result rs004 = convert< string>::from(-5, const_c_str);
    convert<wstring>::result rs005 = convert<wstring>::from(-5, wstd_str);
    convert<wstring>::result rs006 = convert<wstring>::from(-5, wc_str);
    convert< string>::result rs007 = convert< string>::from(-5, array_str);
    convert< string>::result rs008 = convert< string>::from(-5, vector_str);
    convert< string>::result rs009 = convert< string>::from(-5, list_str);

    BOOST_ASSERT(s001 ==  "-5"); BOOST_ASSERT(rs001 && rs001.value() ==  "-5");
    BOOST_ASSERT(s002 ==  "-5"); BOOST_ASSERT(rs002 && rs002.value() ==  "-5");
    BOOST_ASSERT(s003 ==  "-5"); BOOST_ASSERT(rs003 && rs003.value() ==  "-5");
    BOOST_ASSERT(s004 ==  "-5"); BOOST_ASSERT(rs004 && rs004.value() ==  "-5");
    BOOST_ASSERT(s005 == L"-5"); BOOST_ASSERT(rs005 && rs005.value() == L"-5");
    BOOST_ASSERT(s006 == L"-5"); BOOST_ASSERT(rs006 && rs006.value() == L"-5");
    BOOST_ASSERT(s007 ==  "-5"); BOOST_ASSERT(rs007 && rs007.value() ==  "-5");
    BOOST_ASSERT(s008 ==  "-5"); BOOST_ASSERT(rs008 && rs008.value() ==  "-5");
    BOOST_ASSERT(s009 ==  "-5"); BOOST_ASSERT(rs009 && rs009.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s010 = convert< string>::from(-5);
    wstring                   s011 = convert<wstring>::from(-5);
    convert< string>::result rs010 = convert< string>::from(-5);
    convert<wstring>::result rs011 = convert<wstring>::from(-5);

    BOOST_ASSERT( s010 ==  "-5");
    BOOST_ASSERT( s011 == L"-5");
    BOOST_ASSERT(rs010 && rs010.value() ==  "-5");
    BOOST_ASSERT(rs011 && rs011.value() == L"-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversions of a user-defined type
    ////////////////////////////////////////////////////////////////////////////

    direction  up_dir1 = direction::up;
    direction  dn_dir1 = direction::dn;
    string up_dir1_str = convert<string>::from(up_dir1);
    string dn_dir1_str = convert<string>::from(dn_dir1);
    direction  up_dir2 = convert<direction>::from(up_dir1_str, dn_dir1);
    direction  dn_dir2 = convert<direction>::from(dn_dir1_str, up_dir1);
    direction  up_dir3 = convert<direction>::from(up_dir1_str, direction::dn);
    direction  dn_dir3 = convert<direction>::from(dn_dir1_str, direction::up);
//  direction  up_dir9 = convert<direction>::from(up_dir1_str); // Doesn't compile. No def.ctor
//  direction  dn_dir9 = convert<direction>::from(dn_dir1_str); // Doesn't compile. No def.ctor

    convert<direction>::result up_dir4 = convert<direction>::from("junk", dn_dir1);

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
        direction dir = convert<direction>::from("junk", direction::up)(throw_ = true);
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
    char const*  double_s01 = "1.2345e-02"; // Linux
    char const*  double_s02 = "1.2345e-002"; // Windows

    int       hex_v01 = convert<   int>::from(hex1_str, 0) >> std::hex;
    int       hex_v02 = convert<   int>::from(hex2_str, 0) >> std::hex;
    string    hex_s01 = convert<string>::from(hex_v01) >> std::showbase >> std::uppercase >> std::hex;
    string    hex_s02 = convert<string>::from(hex_v02) >> std::showbase >> std::uppercase >> std::hex;
    double double_v01 = convert<double>::from(double_s01) >> std::setprecision(4) >> std::scientific;
    string double_s03 = convert<string>::from(double_v01) >> std::setprecision(4) >> std::scientific;

    BOOST_ASSERT(hex_v01 == 255);
    BOOST_ASSERT(hex_v02 ==  15);
    BOOST_ASSERT(hex_s01 == "0XFF");
    BOOST_ASSERT(hex_s02 ==  "0XF");
    BOOST_ASSERT(double_s03 == double_s01 || double_s03 == double_s02);

    ////////////////////////////////////////////////////////////////////////////
    // Testing locale
    ////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
    char const* rus_locale_name = "Russian_Russia.1251";
    char const*    rus_expected = "1,2345e-002";
    char const*    eng_expected = "1.2345e-002";
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
        string double_s02 = convert<string>::from(double_v01)(locale_ = rus_locale) >> std::setprecision(4) >> std::scientific;
        string double_s03 = convert<string>::from(double_v01)(locale_ = eng_locale) >> std::setprecision(4) >> std::scientific;
//      printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_s02.c_str());
//      printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_s03.c_str());

        BOOST_ASSERT(double_s02 == rus_expected);
        BOOST_ASSERT(double_s03 == eng_expected);
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
            convert<int>::from<string>() >> std::hex);

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
        convert<int>::from<string>()(fallback_ = -1) >> std::hex);

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
        convert<string>::from<int>());

    BOOST_ASSERT(new_strings[0] == "15");
    BOOST_ASSERT(new_strings[1] == "16");
    BOOST_ASSERT(new_strings[2] == "17");
    BOOST_ASSERT(new_strings[3] == "18");
    BOOST_ASSERT(new_strings[4] == "-1");

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-string conversion
    ////////////////////////////////////////////////////////////////////////////

    string  s31 = convert< string>::from(std_str);
    string  s32 = convert< string>::from(c_str);
    wstring s35 = convert<wstring>::from(wstd_str);
    wstring s36 = convert<wstring>::from(wc_str);
    string  s37 = convert< string>::from(array_str);

    BOOST_ASSERT(s31 ==  "-11");
    BOOST_ASSERT(s32 ==  "-12");
    BOOST_ASSERT(s35 == L"-15");
    BOOST_ASSERT(s36 == L"-16");
    BOOST_ASSERT(s37 ==  "-17");

    ////////////////////////////////////////////////////////////////////////////
    // Testing custom manipulators.
    // I do not really know how to write manipulators.
    // Wrote my_cypher just to demonstrate the idea.
    ////////////////////////////////////////////////////////////////////////////

    string encrypted = convert<string>::from("ABC") >> my_cypher;
    string decrypted = convert<string>::from(encrypted) >> my_cypher;

    BOOST_ASSERT(encrypted == "123");
    BOOST_ASSERT(decrypted == "ABC");

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

    convert<bool>::result bool_res1 = convert<bool>::from("1",     false);
    convert<bool>::result bool_res2 = convert<bool>::from("true",  false);
    convert<bool>::result bool_res3 = convert<bool>::from("yes",   false);
    convert<bool>::result bool_res4 = convert<bool>::from(L"1",    false);
    convert<bool>::result bool_res5 = convert<bool>::from(L"true", false);
    convert<bool>::result bool_res6 = convert<bool>::from(L"yes",  false);
    convert<bool>::result bool_res7 = convert<bool>::from("junk",   true);

    BOOST_ASSERT( bool_res1 && bool_res1.value() == true);
    BOOST_ASSERT( bool_res2 && bool_res2.value() == true);
    BOOST_ASSERT( bool_res3 && bool_res3.value() == true);
    BOOST_ASSERT( bool_res4 && bool_res4.value() == true);
    BOOST_ASSERT( bool_res5 && bool_res5.value() == true);
    BOOST_ASSERT( bool_res6 && bool_res6.value() == true);
    BOOST_ASSERT(!bool_res7 && bool_res7.value() == true);

    string bool_str1 = convert<string>::from(true,  "failed");
    string bool_str2 = convert<string>::from(false, "failed");
    string bool_str3 = convert<string>::from(1,     "failed");
    string bool_str4 = convert<string>::from(0,     "failed");

    BOOST_ASSERT(bool_str1 == "1");
    BOOST_ASSERT(bool_str2 == "0");
    BOOST_ASSERT(bool_str3 == "1");
    BOOST_ASSERT(bool_str4 == "0");

    ////////////////////////////////////////////////////////////////////////////
    // Testing direct access to converter.
    // Converter is pre-configured (to apply hex) once and then re-used.
    ////////////////////////////////////////////////////////////////////////////

//  convert<int>::converter<string> dec_cvt = convert<int>::from(string(), -1); // Same as below
    convert<int>::converter<string> dec_cvt = convert<int>::from<string>()(fallback_ = -1);
    convert<int>::converter<string> hex_cvt = convert<int>::from<string>() >> std::hex;

    int cvt_result11 = dec_cvt("F"); // Conversion fails
    int cvt_result21 = hex_cvt("F");
    int cvt_result12 = dec_cvt("10"); // Returns 10
    int cvt_result22 = hex_cvt("10"); // Returns 16

    BOOST_ASSERT(cvt_result11 == -1); // Failed conversion
    BOOST_ASSERT(cvt_result12 == 10);
    BOOST_ASSERT(cvt_result21 == 15);
    BOOST_ASSERT(cvt_result22 == 16);

    ////////////////////////////////////////////////////////////////////////////
    // Work in progress.
    ////////////////////////////////////////////////////////////////////////////
//    convert<>::format format;

    printf("Test completed successfully.\n");
    return 0;
}
