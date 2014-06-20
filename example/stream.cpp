#include "../test/test.hpp"
#include "./example.hpp"
//[stream_headers1
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>

using std::string;
//]

//[stream_headers2
namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;
//]

static
void
example1()
{
    //[stream_example1
    boost::cnv::cstream cnv;

    int    i2 = boost::convert<int>("123", cnv).value(); // Throws if the conversion fails
    int    i3 = boost::convert<int>("uhm", cnv).value_or(-1); // Returns -1 if the conversion fails
    string s2 = boost::convert<string>(123, cnv).value();

    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    BOOST_TEST(s2 == "123");
    //]
}

static
void
example2()
{
    //[stream_example2
    boost::cnv::cstream ccnv;
    boost::cnv::wstream wcnv;

    int v01 = boost::convert<int>("  FF", ccnv(std::hex)(std::skipws)).value_or(0);
    int v02 = boost::convert<int>(L"  F", wcnv(std::hex)(std::skipws)).value_or(0);
    int v03 = boost::convert<int>("  FF", ccnv(std::dec)(std::skipws)).value_or(-5);
    int v04 = boost::convert<int>(L"  F", wcnv(std::dec)(std::skipws)).value_or(-5);

    BOOST_TEST(v01 == 255); // "FF"
    BOOST_TEST(v02 ==  15); // L"F"
    BOOST_TEST(v03 ==  -5); // Failed to convert "FF" as decimal.
    BOOST_TEST(v04 ==  -5); // Failed to convert L"F" as decimal.
    //]
    //[stream_example3
    ccnv(std::showbase)(std::uppercase)(std::hex);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "0XF");
    //]
    //[stream_example4
    ccnv(arg::base = cnv::base::dec)
        (arg::uppercase = false)
        (arg::notation = cnv::notation::scientific);
    //]
    //[stream_example5
    ccnv(std::dec)(std::uppercase)(std::scientific);
    //]
}

static
void
example6()
{
    //[stream_example6
    boost::cnv::cstream cnv;

    change up = boost::convert<change>("up", cnv).value();
    string s1 = boost::convert<string>(up, cnv).value();
    string s2 = boost::convert<string, change>(change::dn, cnv).value();

    BOOST_TEST(up.value() == change::up);
    BOOST_TEST(s1 == "up");
    BOOST_TEST(s2 == "dn");
    //]
}

void
example::stream()
{
    example1();
    example2();
    example6();
}
