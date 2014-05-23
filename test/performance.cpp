// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

template<typename Converter>
double
performance_string_to_type(Converter const& try_converter)
{
    char const* input[] = { "no", "up", "dn" };
    int             sum = 0;
    double           p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
    {
        change chg = boost::convert<change>(input[k % 3], try_converter).value();
        int    res = chg.value();

        BOOST_TEST(res == k % 3);

        sum += res; // Make sure chg is not optimized out
    }
    double   p2 = clock();
    int use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
performance_type_to_string(Converter const& try_converter)
{
    boost::array<change, 3>   input = {{ change::no, change::up, change::dn }};
    boost::array<string, 3> results = {{ "no", "up", "dn" }};
    int                         sum = 0;
    double                       p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
    {
        string res = boost::convert<string>(input[k % 3], try_converter).value();

        BOOST_TEST(res == results[k % 3]);

        sum += res[0]; // Make sure res is not optimized out
    }
    double   p2 = clock();
    int use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

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
        sum += boost::convert<int>((str[4 - k % 5] = 49 + k % 9, str), try_converter).value();

    double   p2 = clock();
    int use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
performance_int_to_string(Converter const& try_converter)
{
    std::vector<std::string> res; res.reserve(test::num_cycles);
    double                    p1 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        res.push_back(boost::convert<std::string>(k, try_converter).value());

    double p2 = clock();

    for (int k = 0; k < test::num_cycles; ++k)
        BOOST_TEST(k == boost::convert<int>(res[k], try_converter).value());

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

	printf("int-to-str: ltostr/prntf/lcast/sstream=%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance_int_to_string(boost::       strtol_converter()),
           performance_int_to_string(boost::       printf_converter()),
           performance_int_to_string(boost:: lexical_cast_converter()),
           performance_int_to_string(boost::cstringstream_converter()));

    printf("str-to-user-type: lcast/sstream=%.2f/%.2f seconds.\n",
           performance_string_to_type(boost:: lexical_cast_converter()),
           performance_string_to_type(boost::cstringstream_converter()));
    printf("user-type-to-str: lcast/sstream=%.2f/%.2f seconds.\n",
           performance_type_to_string(boost:: lexical_cast_converter()),
           performance_type_to_string(boost::cstringstream_converter()));
}
