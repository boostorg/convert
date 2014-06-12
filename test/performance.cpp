// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert/spirit.hpp>

using std::string;

inline
int
str_to_int_spirit(std::string const& str)
{
    std::string::const_iterator beg = str.begin();
    std::string::const_iterator end = str.end();
    int result;

    if (boost::spirit::qi::parse(beg, end, boost::spirit::int_, result))
        if (beg == end) // ensure the whole string was parsed
            return result;

    return (BOOST_ASSERT(0), result);
}

static
double
performance_string_to_int_spirit(test::cnv::strings const& strings)
{
    int         sum = 0;
    int const  size = strings.size();
    double const p1 = clock();

    for (int k = 0; k < size; ++k)
        sum += str_to_int_spirit(strings[k]);

    double const   p2 = clock();
    int const use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
test::performance::str_to_int(test::cnv::strings const& strings, Converter const& try_converter)
{
    int         sum = 0;
    int const  size = strings.size();
    double const p1 = clock();

    for (int k = 0; k < size; ++k)
        sum += boost::convert<int>(strings[k], try_converter).value();

    double const   p2 = clock();
    int const use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

    return (p2 - p1) / CLOCKS_PER_SEC + use_sum;
}

template<typename Converter>
double
performance_string_to_type(Converter const& try_converter)
{
    char const* input[] = { "no", "up", "dn" };
    int             sum = 0;
    double           p1 = clock();

    for (int k = 0; k < test::cnv::num_cycles; ++k)
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

    for (int k = 0; k < test::cnv::num_cycles; ++k)
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
test::performance::int_to_str(test::cnv::ints const& ints, Converter const& try_converter)
{
    int const                   size = ints.size();
    std::vector<std::string> strings; strings.reserve(size);
    double const                  p1 = clock();

    for (int k = 0; k < size; ++k)
        strings.push_back(boost::convert<std::string>(ints[k], try_converter).value());

    double const p2 = clock();

    for (int k = 0; k < size; ++k)
        BOOST_TEST(ints[k] == boost::convert<int>(strings[k], try_converter).value());

    return (p2 - p1) / CLOCKS_PER_SEC;
}

void
test::cnv::performance(test::cnv::strings const& strings, test::cnv::ints const& ints)
{
    int const num_tries = 5;

    for (int k = 0; k < num_tries; ++k)
    {
        double cnv_time = performance::str_to_int(strings, boost::cnv::spirit());
        double raw_time = performance_string_to_int_spirit(strings);

        printf("str-to-int spirit: raw/cnv=%.2f/%.2f seconds (%.2f%%).\n",
               raw_time,
               cnv_time,
               100 * cnv_time / raw_time);
    }
    printf("str-to-int: strtol/scanf/lcast/sstream=%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance::str_to_int(strings, boost::cnv::strtol()),
           performance::str_to_int(strings, boost::cnv::printf()),
           performance::str_to_int(strings, boost::cnv::lexical_cast()),
           performance::str_to_int(strings, boost::cnv::cstringstream()));

    printf("int-to-str: ltostr/prntf/lcast/sstream=%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance::int_to_str(ints, boost::cnv::strtol()),
           performance::int_to_str(ints, boost::cnv::printf()),
           performance::int_to_str(ints, boost::cnv::lexical_cast()),
           performance::int_to_str(ints, boost::cnv::cstringstream()));

    printf("str-to-user-type: lcast/sstream=%.2f/%.2f seconds.\n",
           performance_string_to_type(boost::cnv::lexical_cast()),
           performance_string_to_type(boost::cnv::cstringstream()));
    printf("user-type-to-str: lcast/sstream=%.2f/%.2f seconds.\n",
           performance_type_to_string(boost::cnv::lexical_cast()),
           performance_type_to_string(boost::cnv::cstringstream()));
}
