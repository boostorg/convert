// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;

template<typename Converter>
static
void
test_invalid(Converter const& cnv)
{
    char const* str[] = { "not", "1 2", " 33", "44 ", "0x11", "7 + 5" };
    int const    size = sizeof(str) / sizeof(str[0]);

    for (int k = 0; k < size; ++k)
    {
        boost::optional<int> const res = boost::convert<int>(str[k], cnv);
        bool const                   failed = !res;

        if (!failed)
        {
            printf("test::cnv::invalid() failed for: <%s><%d>\n", str[k], res.value());
        }
        BOOST_TEST(failed);
    }
}

void
test::cnv::lcast_converter()
{
    boost::cnv::lexical_cast cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";
    int const            v00 = boost::convert<int>(not_int_str, cnv).value_or(-1);
    int const            v01 = boost::convert<int>(std_str,     cnv).value_or(-1);
    int const            v02 = boost::convert<int>(c_str,       cnv).value_or(-1);

    BOOST_TEST(v00 ==  -1); // Failed conversion. No throw
    BOOST_TEST(v01 == -11);
    BOOST_TEST(v02 == -12);

    test_invalid(boost::cnv::lexical_cast());
}
