// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "./timer.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;

struct str_to_int_spirit
{
    int operator()(char const* str) const
    {
        char const* beg = str;
        char const* end = beg + strlen(str);
        int result;

        if (boost::spirit::qi::parse(beg, end, boost::spirit::int_, result))
            if (beg == end) // ensure the whole string was parsed
                return result;

        return (BOOST_ASSERT(0), result);
    }
};

struct str_to_int_lxcast
{
    int operator()(char const* str) const
    {
        return boost::lexical_cast<int>(str);
    }
};

template<typename Converter>
double
performance_str_to_int(Converter const& cnv)
{
    test::cnv::strings strings = test::cnv::get_strs(); // Create strings on the stack
    int const             size = strings.size();
    int                    sum = 0;
    test::cnv::timer             timer (sum);

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            sum += cnv(strings[k].c_str());

    return timer.value();
}

template<typename Converter>
double
test::performance::str_to_int(Converter const& try_converter)
{
    test::cnv::strings strings = test::cnv::get_strs(); // Create strings on the stack
    int const             size = strings.size();
    int                    sum = 0;
    test::cnv::timer     timer (sum);

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            sum += boost::convert<int>(strings[k].c_str(), try_converter).value();

    return timer.value();
}

template<typename Converter>
double
test::performance::int_to_str(Converter const& try_converter)
{
    test::cnv::ints   ints = test::cnv::get_ints();
    int const         size = ints.size();
    int                sum = 0;
    test::cnv::timer timer (sum);

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            sum += boost::convert<std::string>(ints[k], try_converter).value()[0];

    return timer.value();
}

template<typename Converter>
double
performance_str_to_type(Converter const& try_converter)
{
    char const*    input[] = { "no", "up", "dn" };
    int                sum = 0;
    test::cnv::timer timer (sum);

    for (int k = 0; k < test::cnv::num_cycles; ++k)
    {
        change chg = boost::convert<change>(input[k % 3], try_converter).value();
        int    res = chg.value();

        BOOST_TEST(res == k % 3);

        sum += res; // Make sure chg is not optimized out
    }
    return timer.value();
}

template<typename Converter>
double
performance_type_to_str(Converter const& try_converter)
{
    boost::array<change, 3>   input = {{ change::no, change::up, change::dn }};
    boost::array<string, 3> results = {{ "no", "up", "dn" }};
    int                         sum = 0;
    test::cnv::timer           timer (sum);

    for (int k = 0; k < test::cnv::num_cycles; ++k)
    {
        string res = boost::convert<string>(input[k % 3], try_converter).value();

        BOOST_TEST(res == results[k % 3]);

        sum += res[0]; // Make sure res is not optimized out
    }
    return timer.value();
}

void
test::cnv::performance()
{
    int const num_tries = 5;

    for (int k = 0; k < num_tries; ++k)
    {
        double cnv_time = performance::str_to_int(boost::cnv::spirit());
        double raw_time = performance_str_to_int(str_to_int_spirit());
        double   change = 100 * (1 - cnv_time / raw_time);

        printf("str-to-int: spirit raw/cnv=%.2f/%.2f seconds (%.2f%%).\n", raw_time, cnv_time, change);
    }
    for (int k = 0; k < num_tries; ++k)
    {
        double cnv_time = performance::str_to_int(boost::cnv::lexical_cast());
        double raw_time = performance_str_to_int(str_to_int_lxcast());
        double   change = 100 * (1 - cnv_time / raw_time);

        printf("str-to-int: lxcast raw/cnv=%.2f/%.2f seconds (%.2f%%).\n", raw_time, cnv_time, change);
    }
    printf("str-to-int: spirit/strtol/scanf/lcast/stream=%.2f/%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance::str_to_int(boost::cnv::spirit()),
           performance::str_to_int(boost::cnv::strtol()),
           performance::str_to_int(boost::cnv::printf()),
           performance::str_to_int(boost::cnv::lexical_cast()),
           performance::str_to_int(boost::cnv::cstream()));

    printf("int-to-str: spirit/ltostr/prntf/lcast/stream=NA/%.2f/%.2f/%.2f/%.2f seconds.\n",
           performance::int_to_str(boost::cnv::strtol()),
           performance::int_to_str(boost::cnv::printf()),
           performance::int_to_str(boost::cnv::lexical_cast()),
           performance::int_to_str(boost::cnv::cstream()));

    printf("str-to-user-type: lcast/stream=%.2f/%.2f seconds.\n",
           performance_str_to_type(boost::cnv::lexical_cast()),
           performance_str_to_type(boost::cnv::cstream()));
    printf("user-type-to-str: lcast/stream=%.2f/%.2f seconds.\n",
           performance_type_to_str(boost::cnv::lexical_cast()),
           performance_type_to_str(boost::cnv::cstream()));
}
