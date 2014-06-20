#include "./example.hpp"

static
void
log(char const*)
{
}

//[getting_started_headers1
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>

using std::string;
//]
static void getting_started_example1()
{
    //[getting_started_example1
    try
    {
        boost::cnv::lexical_cast cnv;

        int    i1 = boost::lexical_cast<int>("123");
        int    i2 = boost::convert<int>("123", cnv).value();
        string s1 = boost::lexical_cast<string>(123);
        string s2 = boost::convert<string>(123, cnv).value();

        BOOST_TEST(i1 == 123);
        BOOST_TEST(i2 == 123);
        BOOST_TEST(s1 == "123");
        BOOST_TEST(s2 == "123");
    }
    catch (std::exception const&)
    {
        // Need to be aware and prepared that the conversion requests above can fail.
        // Ignore this at your peril.
    }
    //]
}

static void getting_started_example2()
{
    //[getting_started_example2
    boost::cnv::lexical_cast cnv;

    // Do not throw on conversion failure but return fallback values instead.
    int    i2 = boost::convert<int>("123", cnv).value_or(-1);
    string s2 = boost::convert<string>(123, cnv).value_or("bad");

    BOOST_TEST(i2 == 123);
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

    int i1 = boost::lexical_cast<int>("123");
    int i2 = boost::convert<int>("123", cnv).value(); // Five times faster than lexical_cast
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
        int i1 = boost::lexical_cast<int>("   123"); // Does not work
        BOOST_TEST(!"Never reached");
    }
    catch (...) {}

    int    i2 = boost::convert<int>("   123", cnv(std::skipws)).value(); // Success
    string s1 = boost::lexical_cast<string>(123.456);
    string s2 = boost::convert<string>(123.456, cnv(std::setprecision(4))).value();

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

    int i1 = boost::lexical_cast<int>("123"); // Throws if the conversion fails
    int i2 = boost::convert<int>("123", cnv).value(); // Throws if the conversion fails
    int i3 = boost::convert<int>("uhm", cnv).value_or(-1); // Returns -1 if the conversion fails

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    //]
}

static void getting_started_example6()
{
    std::string const        str1 = "123";
    std::string const        str2 = "456";
    int const default_num_threads = 11;
    int const default_num_windows = 12;
    boost::cnv::cstream       cnv;

    //[getting_started_example6

    int num_threads = boost::convert<int>(str1, cnv(std::hex)).value_or(INT_MAX); // Read as hex
    int num_windows = boost::convert<int>(str2, cnv(std::dec)).value_or(INT_MAX); // Read as decimal

    if (num_threads == INT_MAX) log("bad num_threads"), num_threads = default_num_threads;
    if (num_windows == INT_MAX) log("bad num_windows"), num_windows = default_num_windows;

    // ... proceed
    //]
    BOOST_TEST(num_threads == 291);
    BOOST_TEST(num_windows == 456);
}

static void getting_started_example7()
{
    std::string const        str1 = "123";
    std::string const        str2 = "456";
    int const default_num_threads = 11;
    int const default_num_windows = 12;
    boost::cnv::cstream       cnv;

    //[getting_started_example7

    int num_threads = boost::convert<int>(str1, cnv(std::hex)).value_or(default_num_threads);
    int num_windows = boost::convert<int>(str2, cnv(std::dec)).value_or(default_num_windows);

    // ... proceed
    //]
    BOOST_TEST(num_threads == 291);
    BOOST_TEST(num_windows == 456);
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
}
