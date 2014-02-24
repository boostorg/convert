// Boost.Convert library test and usage example
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

template<typename Converter>
double
performance_string_to_type(Converter const& try_converter)
{
    char const* input[] = { "no", "up", "dn" };
    int             sum = 0;
    double           p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
    {
        direction_with_default dir = boost::convert<direction_with_default>::from(input[k % 3], try_converter).value();
        int                    res = dir.value();

        BOOST_ASSERT(res == k % 3);

        sum += res; // Make sure dir is not optimized out
    }
    double   p2 = clock();
    int use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_ASSERT(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
performance_string_to_int(Converter const& try_converter)
{
    char str[] = "12345";
    int    sum = 0;
    double  p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        sum += boost::convert<int>::from((str[4 - k % 5] = 49 + k % 9, str), try_converter).value();

    double   p2 = clock();
    int use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_ASSERT(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
performance_int_to_string(Converter const& try_converter)
{
    std::vector<std::string> res; res.reserve(test::num_cycles);
	double                    p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        res.push_back(boost::convert<std::string>::from(k, try_converter).value());

    double p2 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        BOOST_ASSERT(k == boost::convert<int>::from(res[k], try_converter).value());

    return (p2 - p1) / CLOCKS_PER_SEC;
}

void
test::performance()
{
    printf("str-to-int: strtol/scanf/lcast/sstream=%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance_string_to_int(boost::       strtol_converter()),
           performance_string_to_int(boost::       printf_converter()),
           performance_string_to_int(boost:: lexical_cast_converter()),
           performance_string_to_int(boost::cstringstream_converter()));

	printf("int-to-str: ltostr/scanf/lcast/sstream=%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance_int_to_string(boost::       strtol_converter()),
           performance_int_to_string(boost::       printf_converter()),
           performance_int_to_string(boost:: lexical_cast_converter()),
           performance_int_to_string(boost::cstringstream_converter()));

    printf("str-to-user-type: lcast/sstream=%.2f/%.2f seconds.\n",
           performance_string_to_type(boost:: lexical_cast_converter()),
           performance_string_to_type(boost::cstringstream_converter()));
}
