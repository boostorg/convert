// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "./str_to_int.hpp"
#include "./int_to_str.hpp"

#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/convert/lexical_cast.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace arg = boost::cnv::parameter;

using std::string;
using std::wstring;

void
test::cnv::force_in_type()
{
    boost::cnv::cstream cnv;

    string const      s1 = boost::convert<string>(-1, cnv).value();
    string const      s2 = boost::convert<string, unsigned int>(-1, cnv).value();
    char const* expected = sizeof(unsigned int) == 4 ? "4294967295" : 0;

    if (expected)
    {
        BOOST_TEST(s1 == "-1");
        BOOST_TEST(s2 == expected);
    }
}

int
main(int argc, char const* argv[])
{
    test::cnv::scratchpad();
    test::cnv::sfinae();
    test::cnv::is_converter();
    test::cnv::stream_converter();
    test::cnv::lcast_converter();
    test::cnv::printf_converter();
    test::cnv::strtol_converter();

    test::cnv::str_to_int(boost::cnv::lexical_cast());
    test::cnv::str_to_int(boost::cnv::cstream());
    test::cnv::str_to_int(boost::cnv::strtol());
    test::cnv::str_to_int(boost::cnv::printf());
//    test::cnv::str_to_int(boost::cnv::spirit());

    test::cnv::int_to_str(boost::cnv::lexical_cast());
    test::cnv::int_to_str(boost::cnv::cstream());
    test::cnv::int_to_str(boost::cnv::strtol());
    test::cnv::int_to_str(boost::cnv::printf());
//    test::cnv::int_to_str(boost::cnv::spirit());

    test::cnv::user_type();
    test::cnv::force_in_type();
    test::cnv::callables();
    test::cnv::fallbacks();
    test::cnv::encryption();
//    test::cnv::performance();

    return boost::report_errors();
}
