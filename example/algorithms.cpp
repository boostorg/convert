// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.



#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/convert/stream.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <vector>

#include "./example.hpp"

static
void
introduction()
{
//[algorithm_introduction

   /*`For example, the following code converts an array of integers from their textual hexadecimal
      representation and assigns -1 to those which fail to convert:
   */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstream           cnv;

    cnv(std::hex)(std::skipws); // Instruct to read as string of hexadecimal characters, and skip (leading) white spaces.

    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(boost::cref(cnv)).value_or(-1));

    BOOST_TEST(ints.size() == 3); // Number of values read.
    BOOST_TEST(ints[0] ==  5); // " 5"
    BOOST_TEST(ints[1] == 15); // "0XF"
    BOOST_TEST(ints[2] == -1); // Returned value from a failed conversion of "not an int".
//] //[/algorithm_introduction]

}

static
void
example1()
{
    //[algorithm_example1
    /*`The following code demonstrates a failed attempt to convert a few `string`s to `int`s
    with `boost::lexical_cast` (and one of the reasons ['Boost.Convert] has been developed):
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

           BOOST_TEST(0 && "Never reached!");
    }
    catch (std::exception&)
    {
        BOOST_TEST(ints.size() == 0); // No strings converted.
    }
    //]
}

static
void
example2()
{
    //[algorithm_example2
    /*`If the exception-throwing behavior is the desired behavior, then ['Boost.Convert] supports that.

    Alternatively, it offers a nothrow process-flow returning instead INT_MAX using the same `boost::lexical_cast` converter:
    */
    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::lexical_cast      cnv;

    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(cnv).value_or(INT_MAX));

    // No exceptions thrown.

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] == INT_MAX); // Failed conversion.
    BOOST_TEST(ints[1] == INT_MAX); // Failed conversion.
    BOOST_TEST(ints[2] == INT_MAX); // Failed conversion.
    //]
}

static
void
example3()
{
    //[algorithm_example3
    /*`Replacing the original `boost::lexical_cast`-based converter yields better results with exception-throwing
       and non-throwing process-flows supported:
    */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstream           cnv;

    try
    {
      std::transform(
           strs.begin(),
           strs.end(),
           std::back_inserter(ints),
           boost::convert<int, std::string>(boost::cref(cnv(std::hex)(std::skipws))));

           BOOST_TEST(0 && "Never reached!");
    }
    catch (boost::bad_optional_access const&)
    {
        BOOST_TEST(ints.size() == 2); // Only the first two strings converted.
        BOOST_TEST(ints[0] ==  5); // " 5"
        BOOST_TEST(ints[1] == 15); // "0XF"

        // "not an int" is effectively ignored.
    }
    //]
}

static
void
example4()
{
    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstream           cnv;

    //[algorithm_example4
    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(boost::cref(cnv(std::hex)(std::skipws))).value_or(-1));

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] ==  5);
    BOOST_TEST(ints[1] == 15);
    BOOST_TEST(ints[2] == -1); // Failed conversion

    /*`[important One notable difference in the deployment of `boost::cnv::cstream` with algorithms is
       the use of `boost::cref` (or `std::ref` in C++11).

       It needs to be remembered that with standard algorithms the deployed converter needs to be
       [@http://en.cppreference.com/w/cpp/concept/TriviallyCopyable copyable] or
       [@http://en.cppreference.com/w/cpp/concept/MoveAssignable moveable (C++11)]
       and is, in fact, copied or moved by the respective algorithm before being used.
       Given that `std::cstringstream` is not copyable, `boost::cnv::cstream` is not copyable either.
       That limitation is routinely worked-around using `boost::ref`.]
    */
    //]
}

static
void
example5()
{
//[algorithm_example5
    /*`And, lastly, an example of algorithm-based integer-to-string formatted conversion with
       `std::hex`, `std::uppercase` and `std::showbase` formatting applied:
    */
    boost::array<int, 3>     ints = {{ 15, 16, 17 }};
    std::vector<std::string> strs;
    boost::cnv::cstream       cnv;

    cnv(std::hex)(std::uppercase)(std::showbase);

    std::transform(
        ints.begin(),
        ints.end(),
        std::back_inserter(strs),
        boost::convert<std::string, int>(boost::cref(cnv)));

    BOOST_TEST(strs.size() == 3);
    BOOST_TEST(strs[0] ==  "0XF"); // 15
    BOOST_TEST(strs[1] == "0X10"); // 16
    BOOST_TEST(strs[2] == "0X11"); // 17
//] [/algorithm_example5]
}

void
example::algorithms()
{
    introduction();
    example1();
    example2();
    example3();
    example4();
    example5();
}

int main()
{
  example::algorithms();
}