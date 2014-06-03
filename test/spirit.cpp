// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert/spirit.hpp>

static
int
str_to_int_spirit(std::string const& str)
{
    std::string::const_iterator i = str.begin();
    int result;

    if (!boost::spirit::qi::parse(i, str.end(), boost::spirit::int_, result))
        BOOST_ASSERT(0);

    BOOST_ASSERT(i == str.end()); // ensure the whole string was parsed

    return result;
}

double
performance_string_to_int_spirit()
{
    std::string str = "12345";
    int         sum = 0;
    double const p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        sum += str_to_int_spirit((str[4 - k % 5] = 49 + k % 9, str));

    double const   p2 = clock();
    int const use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

void
test::spirit()
{
    printf("str-to-int spirit: raw/cnv=%.2f/%.2f seconds.\n",
           performance_string_to_int_spirit(),
           performance_string_to_int(boost::spirit_converter()));
}
