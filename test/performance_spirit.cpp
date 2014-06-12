// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

// This code has been adapted from libs/spirit/optimization/qi/int_parser.cpp.
// This code uses the performance testing framework from libs/spirit/optimization/measure.cpp.
// Copyright (c) 2001-2010 Joel de Guzman

#include "./test.hpp"
#include <boost/convert/spirit.hpp>
#define main() old_test_spirit()
#include <libs/spirit/optimization/qi/int_parser.cpp>

using std::string;

namespace { namespace local
{
    test::cnv::strings strings;

    struct strtol_test : test::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < local::strings.size(); ++i)
            {
                std::string const& str = local::strings[i];
                char*              end = 0;

                this->val += ::strtol(str.c_str(), &end, 10);
            }
        }
    };
    struct spirit_test : test::base
    {
        static int parse(std::string const& str)
        {
            std::string::const_iterator beg = str.begin();
            std::string::const_iterator end = str.end();
            int n;

            if (boost::spirit::qi::parse(beg, end, boost::spirit::qi::int_, n))
                if (beg == end)
                    return n;

            return (BOOST_ASSERT(0), 0);
        }
        void benchmark()
        {
            for (size_t i = 0; i < local::strings.size(); ++i)
                this->val += parse(local::strings[i]);
        }
    };
    struct cnv_test : test::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < local::strings.size(); ++i)
                this->val += boost::convert<int>(local::strings[i], boost::cnv::spirit()).value();
        }
    };
}}

int
test::performance::spirit_framework(test::cnv::strings const& strings)
{
    local::strings = strings; local::strings.resize(1000);

    // This code has been adapted and uses the performance testing framework from
    // libs/spirit/optimization/qi/int_parser.cpp
    // Copyright (c) 2001-2010 Joel de Guzman

    BOOST_SPIRIT_TEST_BENCHMARK(
        10000,     // This is the maximum repetitions to execute
        (local::strtol_test)
        (local::spirit_test)
        (local::cnv_test)
    )

    // This is ultimately responsible for preventing all the test code
    // from being optimized away.  Change this to return 0 and you
    // unplug the whole test's life support system.
    return test::live_code != 0;
}

