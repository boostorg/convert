// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./example.hpp"

static
void
log(char const*)
{
}

//[getting_started_headers1
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
//]
//[using_header
using std::string;
using boost::lexical_cast;
using boost::convert;
//]
static void getting_started_example1()
{
    //[getting_started_example1
    try
    {
        boost::cnv::lexical_cast cnv;

        int    i1 = lexical_cast<int>("123");
        int    i2 = convert<int>("123", cnv).value();
        string s1 = lexical_cast<string>(123);
        string s2 = convert<string>(123, cnv).value();

        BOOST_TEST(i1 == 123);
        BOOST_TEST(i2 == 123);
        BOOST_TEST(s1 == "123");
        BOOST_TEST(s2 == "123");
    }
    catch (std::exception const&)
    {
        // Be aware and prepared that the conversion requests above can fail.
        // Ignore this at your peril.
    }
    //]
}

static void getting_started_example2()
{
    //[getting_started_example2
    boost::cnv::lexical_cast cnv;

    // Non-throwing behavior. Return fallback values when failed.
    int    i2 = convert<int>("uhm", cnv).value_or(-1);
    string s2 = convert<string>(123, cnv).value_or("bad");

    BOOST_TEST(i2 == -1);
    BOOST_TEST(s2 == "123");
    //]
}

//[getting_started_headers3
#include <boost/convert/spirit.hpp>
//]
static void getting_started_example3()
{
    //[getting_started_example3
    boost::cnv::spirit cnv;

    int i1 = lexical_cast<int>("123");
    int i2 = convert<int>("123", cnv).value(); // Five times faster
    //]
}

//[getting_started_headers4
#include <boost/convert/stream.hpp>
//]
static void getting_started_example4()
{
    //[getting_started_example4
    boost::cnv::cstream cnv;

    try
    {
        int i1 = lexical_cast<int>("   123"); // Does not work
        BOOST_TEST(!"Never reached");
    }
    catch (...) {}

    int    i2 = convert<int>("   123", cnv(std::skipws)).value(); // Success
    string s1 = lexical_cast<string>(123.456);
    string s2 = convert<string>(123.456, cnv(std::setprecision(4))).value();

    BOOST_TEST(i2 == 123);
    BOOST_TEST(s1 == "123.456"); // The only available output format
    BOOST_TEST(s2 == "123.5");   // Precision was set to 4. Correctly rounded.
    //]
}

static
void
getting_started_example5()
{
    //[getting_started_example5
    boost::cnv::cstream cnv;

    int i1 = lexical_cast<int>("123"); // Throws when fails
    int i2 = convert<int>("123", cnv).value(); // Throws when fails
    int i3 = convert<int>("uhm", cnv).value_or(-1); // Returns -1 when fails

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    //]
}

static void getting_started_example6()
{
    std::string const    str1 = "123";
    std::string const    str2 = "456";
    int const default_num_hex = 11;
    int const default_num_dec = 12;
    boost::cnv::cstream   cnv;

    //[getting_started_example6

    int num_hex = convert<int>(str1, cnv(std::hex)).value_or(-1); // Read as hex
    int num_dec = convert<int>(str2, cnv(std::dec)).value_or(-1); // Read as decimal

    if (num_hex == -1) log("bad num_hex"), num_hex = default_num_hex;
    if (num_dec == -1) log("bad num_dec"), num_dec = default_num_dec;

    // ... proceed
    //]
    BOOST_TEST(num_hex == 291);
    BOOST_TEST(num_dec == 456);
}

static void getting_started_example7()
{
    std::string const    str1 = "123";
    std::string const    str2 = "456";
    int const default_num_hex = 11;
    int const default_num_dec = 12;
    boost::cnv::cstream   cnv;

    //[getting_started_example7

    int num_hex = convert<int>(str1, cnv(std::hex)).value_or(default_num_hex);
    int num_dec = convert<int>(str2, cnv(std::dec)).value_or(default_num_dec);

    // ... proceed
    //]
    BOOST_TEST(num_hex == 291);
    BOOST_TEST(num_dec == 456);
}

static void getting_started_example8()
{
    std::string const     str = "123";
    int const default_num_dec = 12;
    //[getting_started_example8
    int num_dec = default_num_dec;

    try
    {
        num_dec = lexical_cast<int>(str);
    }
    catch (...)
    {
        log("bad num_dec");
    }
    //]
    BOOST_TEST(num_dec == 123);
}

void
example::getting_started()
{
    getting_started_example1();
    getting_started_example2();
    getting_started_example3();
    getting_started_example4();
    getting_started_example5();
    getting_started_example6();
    getting_started_example7();
    getting_started_example8();
}
