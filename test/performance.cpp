// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/timer/timer.hpp>

using std::string;
using boost::convert;

namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;

namespace { namespace local
{
    int sum = 0;

    struct timer : public boost::timer::cpu_timer
    {
        typedef timer                   this_type;
        typedef boost::timer::cpu_timer base_type;

        double value() const
        {
            boost::timer::cpu_times times = base_type::elapsed();
            int const             use_sum = (sum % 2) ? 0 : (sum % 2); BOOST_TEST(use_sum == 0);

            return double(times.user + times.system) / 1000000000 + use_sum;
        }
    };
}}

struct str_to_int_spirit
{
    int operator()(char const* str) const
    {
        char const* beg = str;
        char const* end = beg + strlen(str);
        int      result;

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
    local::timer         timer;

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            local::sum += cnv(strings[k].c_str());

    return timer.value();
}

template<typename Type, typename Converter>
double
test::performance::str_to(Converter const& try_converter)
{
    test::cnv::strings strings = test::cnv::get_strs(); // Create strings on the stack
    int const             size = strings.size();
    local::timer         timer;

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            local::sum += boost::convert<Type>(strings[k].c_str(), try_converter).value();

    return timer.value();
}

template<typename Type, typename Converter>
double
test::performance::to_str(Converter const& try_converter)
{
    typedef typename test::cnv::array<Type>::type collection;

    collection  values = test::cnv::get<Type>();
    int const     size = values.size();
    local::timer timer;

    for (int t = 0; t < test::cnv::num_cycles; ++t)
        for (int k = 0; k < size; ++k)
            local::sum += boost::convert<std::string>(Type(values[k]), try_converter).value()[0];

    return timer.value();
}

template<typename Converter>
double
performance_str_to_type(Converter const& try_converter)
{
    char const* input[] = { "no", "up", "dn" };
    local::timer  timer;

    for (int k = 0; k < test::cnv::num_cycles; ++k)
    {
        change chg = boost::convert<change>(input[k % 3], try_converter).value();
        int    res = chg.value();

        BOOST_TEST(res == k % 3);

        local::sum += res; // Make sure chg is not optimized out
    }
    return timer.value();
}

template<typename Converter>
double
performance_type_to_str(Converter const& try_converter)
{
    boost::array<change, 3>   input = {{ change::no, change::up, change::dn }};
    boost::array<string, 3> results = {{ "no", "up", "dn" }};
    local::timer              timer;

    for (int k = 0; k < test::cnv::num_cycles; ++k)
    {
        string res = boost::convert<string>(input[k % 3], try_converter).value();

        BOOST_TEST(res == results[k % 3]);

        local::sum += res[0]; // Make sure res is not optimized out
    }
    return timer.value();
}

template<typename Raw, typename Cnv>
void
performance_comparative(Raw const& raw, Cnv const& cnv, char const* txt)
{
    int const num_tries = 5;
    double     cnv_time = 0;
    double     raw_time = 0;
    double       change = 0;

    for (int k = 0; k < num_tries; ++k)
    {
        raw_time += performance_str_to_int(raw);
        cnv_time += test::performance::str_to<int>(cnv);
        change   += 100 * (1 - cnv_time / raw_time);
    }
    cnv_time /= num_tries;
    raw_time /= num_tries;
    change   /= num_tries;

    printf("str-to-int: %s raw/cnv=%.2f/%.2f seconds (%.2f%%).\n", txt, raw_time, cnv_time, change);
}

static
void
performance_comparative()
{
    performance_comparative(str_to_int_spirit(), boost::cnv::spirit(),       "spirit");
    performance_comparative(str_to_int_lxcast(), boost::cnv::lexical_cast(), "lxcast");
}

void
test::cnv::performance()
{
    printf("Started performance tests...\n");

    printf("str-to-int: spirit/strtoi/lcast/scanf/stream=%8.2f/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
           performance::str_to<int>(boost::cnv::spirit()),
           performance::str_to<int>(boost::cnv::strtol()),
           performance::str_to<int>(boost::cnv::lexical_cast()),
           performance::str_to<int>(boost::cnv::printf()),
           performance::str_to<int>(boost::cnv::cstream()));
    printf("str-to-lng: spirit/strtol/lcast/scanf/stream=%8.2f/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
           performance::str_to<long int>(boost::cnv::spirit()),
           performance::str_to<long int>(boost::cnv::strtol()),
           performance::str_to<long int>(boost::cnv::lexical_cast()),
           performance::str_to<long int>(boost::cnv::printf()),
           performance::str_to<long int>(boost::cnv::cstream()));
    printf("str-to-dbl: spirit/strtod/lcast/scanf/stream=      NA/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
//         performance::str_to<double>(boost::cnv::spirit()),
           performance::str_to<double>(boost::cnv::strtol()),
           performance::str_to<double>(boost::cnv::lexical_cast()),
           performance::str_to<double>(boost::cnv::printf()),
           performance::str_to<double>(boost::cnv::cstream()));

    printf("int-to-str: spirit/itostr/lcast/prntf/stream=%8.2f/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
           performance::to_str<int>(boost::cnv::spirit()),
           performance::to_str<int>(boost::cnv::strtol()),
           performance::to_str<int>(boost::cnv::lexical_cast()),
           performance::to_str<int>(boost::cnv::printf()),
           performance::to_str<int>(boost::cnv::cstream()));
    printf("lng-to-str: spirit/ltostr/lcast/prntf/stream=%8.2f/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
           performance::to_str<long int>(boost::cnv::spirit()),
           performance::to_str<long int>(boost::cnv::strtol()),
           performance::to_str<long int>(boost::cnv::lexical_cast()),
           performance::to_str<long int>(boost::cnv::printf()),
           performance::to_str<long int>(boost::cnv::cstream()));
    printf("dbl-to-str: spirit/dtostr/lcast/prntf/stream=      NA/%8.2f/%8.2f/%8.2f/%8.2f seconds.\n",
//         performance::to_str<double>(boost::cnv::spirit()),
           performance::to_str<double>(boost::cnv::strtol()(arg::precision = 6)),
           performance::to_str<double>(boost::cnv::lexical_cast()),
           performance::to_str<double>(boost::cnv::printf()(arg::precision = 6)),
           performance::to_str<double>(boost::cnv::cstream()(arg::precision = 6)));

    printf("str-to-user-type: lcast/stream=%.2f/%.2f seconds.\n",
           performance_str_to_type(boost::cnv::lexical_cast()),
           performance_str_to_type(boost::cnv::cstream()));
    printf("user-type-to-str: lcast/stream=%.2f/%.2f seconds.\n",
           performance_type_to_str(boost::cnv::lexical_cast()),
           performance_type_to_str(boost::cnv::cstream()));

    if (0)
    {
        performance_comparative();
    }
}
