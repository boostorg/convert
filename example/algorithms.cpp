#include "./example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/sstream.hpp>
#include <boost/array.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <vector>
#include <iomanip>

void
example::algorithm::strings_to_ints_simple()
{
//[algorithm_simple

   /*`For example, the following snippet converts an array of integers from their textual hexadecimal
      representation and assigns INT_MAX to those which fail to convert:
   */

    boost::array<char const*, 5> strings = {{ "0XF", "0X10", "0X11", "0X12", "not an int" }};
    std::vector<int>            integers;
    boost::cstringstream_converter   cnv; // stringstream-based char converter

    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(integers),
        boost::convert<int>(cnv(std::hex)).value_or(INT_MAX));

    BOOST_TEST(integers.size() == 5);
    BOOST_TEST(integers[0] == 15);
    BOOST_TEST(integers[1] == 16);
    BOOST_TEST(integers[2] == 17);
    BOOST_TEST(integers[3] == 18);
    BOOST_TEST(integers[4] == INT_MAX); // Failed conversion
//]
}

