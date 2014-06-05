#include "./example.hpp"
//[getting_started_headers1
#include <boost/convert.hpp>
#include <boost/convert/sstream.hpp>

using std::string;

//]

static
void
log(char const*)
{
}

static
void
getting_started_example1()
{
    //[getting_started_example1

    boost::cnv::cstringstream cnv; // stringstream-based char converter

    int    i1 = boost::lexical_cast<int>("123"); // Throws if the conversion fails
    int    i2 = boost::convert<int>("123", cnv).value(); // Throws if the conversion fails
    int    i3 = boost::convert<int>("uhm", cnv).value_or(-1); // Returns -1 if the conversion fails
    string s1 = boost::convert<string>(123, cnv).value();

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    BOOST_TEST(s1 == "123");

    //]
}

static
void
getting_started_example2()
{
    std::string const             str1 = "123";
    std::string const             str2 = "456";
    int const      default_num_threads = 11;
    int const      default_num_windows = 12;
    boost::cnv::cstringstream cnv; // stringstream-based char converter

    //[getting_started_example2

    int num_threads = boost::convert<int>(str1, cnv(std::hex)).value_or(INT_MAX); // Read as hex
    int num_windows = boost::convert<int>(str2, cnv(std::dec)).value_or(INT_MAX); // Read as decimal

    if (num_threads == INT_MAX) log("bad num_threads"), num_threads = default_num_threads;
    if (num_windows == INT_MAX) log("bad num_windows"), num_windows = default_num_windows;

    // ... proceed
    //]
    BOOST_TEST(num_threads == 291);
    BOOST_TEST(num_windows == 456);
}

static
void
getting_started_example3()
{
    std::string const             str1 = "123";
    std::string const             str2 = "456";
    int const      default_num_threads = 11;
    int const      default_num_windows = 12;
    boost::cnv::cstringstream cnv; // stringstream-based char converter

    //[getting_started_example3

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
}
