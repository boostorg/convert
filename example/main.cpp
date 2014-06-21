// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./example.hpp"
#include <boost/detail/lightweight_test.hpp>

int
main(int argc, char const* argv[])
{
    example::getting_started();
    example::getting_serious();
    example::algorithms();
    example::stream();
    example::lexcast_converter();
    example::default_converter();

    return boost::report_errors();
}
