#include "./example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/sstream.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <vector>

static
void
introduction()
{
//[algorithm_introduction

   /*`For example, the following snippet converts an array of integers from their textual hexadecimal
      representation and assigns INT_MAX to those which fail to convert:
   */

    boost::array<char const*, 3> strings = {{ " 5", "0XF", "not an int" }};
    std::vector<int>            integers;
    boost::cnv::cstringstream   cnv; // stringstream-based char converter

    cnv(std::hex)(std::skipws);

    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(integers),
        boost::convert<int>(cnv).value_or(INT_MAX));

    BOOST_TEST(integers.size() == 3);
    BOOST_TEST(integers[0] ==  5);
    BOOST_TEST(integers[1] == 15);
    BOOST_TEST(integers[2] == INT_MAX); // Failed conversion
//]
}

static
void
example1()
{
    //[algorithm_example1
    /*`The following code demonstrates a failed attempt to convert a few strings with `boost::lexical_cast` (and one of the
       reasons ['Boost.Convert] has been developed):
    */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;

    try
    {
       std::transform(
           strs.begin(),
           strs.end(),
           std::back_inserter(ints),
           boost::bind(boost::lexical_cast<int, std::string>, _1));

           BOOST_TEST(!"Never reached");
    }
    catch (std::exception&)
    {
        // No strings converted.
        BOOST_TEST(ints.size() == 0);
    }
    //]
}

static
void
example2()
{
    //[algorithm_example2
    /*`The first take by ['Boost.Convert] is more successful (it knows how to handle the hexadecimal
      format and to skip white spaces) but still not quite satisfactory (your mileage may vary) as
      conversion is interrupted when ['Boost.Convert] comes across an invalid input:
    */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv;

    try
    {
      std::transform(
           strs.begin(),
           strs.end(),
           std::back_inserter(ints),
           boost::convert<int>(cnv(std::hex)(std::skipws)));

           BOOST_TEST(!"Never reached");
    }
    catch (boost::bad_optional_access const&)
    {
       // Only the first two strings converted. Failed to convert the last one.

        BOOST_TEST(ints.size() == 2);
        BOOST_TEST(ints[0] ==  5);
        BOOST_TEST(ints[1] == 15);
    }
    //]
}

static
void
example3()
{
    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv;

    //[algorithm_example3
    /*`And at last a fully working version (with a conversion-failure fallback value provided):
    */
    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int>(cnv(std::hex)).value_or(-1));

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] ==  5);
    BOOST_TEST(ints[1] == 15);
    BOOST_TEST(ints[2] == -1); // Failed conversion
    //]
}
void
example::algorithms()
{
    introduction();
    example1();
    example2();
    example3();
}
