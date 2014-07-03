// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

// This code has been adapted from libs/spirit/optimization/qi/int_parser.cpp.
// This code uses the performance testing framework from libs/spirit/optimization/measure.cpp.
// See these mentioned files for the copyright notice.

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/convert/lexical_cast.hpp>
#include "./test.hpp"

//#define main() old_str_to_int_test_spirit()
//#include <libs/spirit/optimization/qi/int_parser.cpp>
#include <libs/spirit/workbench/measure.hpp>
#include <string>
#include <vector>
#include <cstdlib>
#include <boost/spirit/include/qi.hpp>

namespace
{
    namespace local
    {
        // C1. 18 = 9 positive + 9 negative numbers with the number of digits from 1 to 9.
        //     Even though INT_MAX(32) = 2147483647, i.e. 10 digits (not to mention long int)
        //     we only test up to 9 digits as Spirit does not handle more than 9.

        typedef boost::array<my_string, 18> strings; //C1
        ///////////////////////////////////////////////////////////////////////////
        // Generate a random number string with N digits
        std::string
        gen_int(int digits, bool negative)
        {
            std::string result;

            if (negative)                       // Prepend a '-'
                result += '-';

            result += '1' + (rand()%9);         // The first digit cannot be '0'

            for (int i = 1; i < digits; ++i)    // Generate the remaining digits
                result += '0' + (rand()%10);
            return result;
        }

        local::strings const&
        get_strs()
        {
            static local::strings strings;
            static bool                filled;
            static bool              negative = true;

            if (!filled)
            {
                // Seed the random generator
                srand(time(0));

                for (size_t k = 0; k < strings.size(); ++k)
                    strings[k] = local::gen_int(k/2 + 1, negative = !negative);

                filled = true;
            }
            return strings;
        }

        struct base : test::base
        {
            base() : strings_(local::get_strs()) {}

            // Test strings are created as part of the object, i.e. on the stack to make sure
            // they are easily accessed.
            local::strings strings_;
        };
    }
    struct raw_lxcast_str_to_int_test : local::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += boost::lexical_cast<int>(strings_[i].c_str());
        }
    };
    struct cnv_lxcast_str_to_int_test : local::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += boost::convert<int>(strings_[i].c_str(), cnv).value();
        }
        boost::cnv::lexical_cast cnv;
    };
    struct raw_strtol_str_to_int_test : local::base
    {
        static int parse(char const* str)
        {
            char const* str_end = str + strlen(str);
            char*       cnv_end = 0;
            long int     result = ::strtol(str, &cnv_end, 10);

            if (INT_MIN <= result && result <= INT_MAX && cnv_end == str_end)
                return int(result);

            return (BOOST_ASSERT(0), 0);
        }
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += parse(strings_[i].c_str());
        }
    };
    struct raw_spirit_str_to_int_test : local::base
    {
        static int parse(char const* str)
        {
            char const* beg = str;
            char const* end = beg + strlen(str);
            int n;

            if (boost::spirit::qi::parse(beg, end, boost::spirit::qi::int_, n))
                if (beg == end)
                    return n;

            return (BOOST_ASSERT(0), 0);
        }
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += parse(strings_[i].c_str());
        }
    };
    struct cnv_strtol_str_to_int_test : local::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += boost::convert<int>(strings_[i].c_str(), cnv).value();
        }
        boost::cnv::strtol cnv;
    };
    struct cnv_spirit_str_to_int_test : local::base
    {
        void benchmark()
        {
            for (size_t i = 0; i < strings_.size(); ++i)
                this->val += boost::convert<int>(strings_[i].c_str(), cnv).value();
        }
        boost::cnv::spirit cnv;
    };
}

int
main(int argc, char const* argv[])
{
    // This code has been adapted from libs/spirit/optimization/qi/int_parser.cpp.
    // This code uses the performance testing framework from libs/spirit/optimization/measure.cpp.
    // See these mentioned files for the copyright notice.

    BOOST_SPIRIT_TEST_BENCHMARK(
        10000000, // This is the maximum repetitions to execute
        (raw_lxcast_str_to_int_test)
        (cnv_lxcast_str_to_int_test)
        (raw_strtol_str_to_int_test)
        (cnv_strtol_str_to_int_test)
        (raw_spirit_str_to_int_test)
        (cnv_spirit_str_to_int_test)
    )

    // This is ultimately responsible for preventing all the test code
    // from being optimized away.  Change this to return 0 and you
    // unplug the whole test's life support system.
    return test::live_code != 0;
}

