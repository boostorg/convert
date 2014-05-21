// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;
using std::wstring;

template<typename Converter>
void
test::type_to_string(Converter const& cnv)
{
    BOOST_TEST("255" == boost::convert<std::string>(255, cnv(arg::base = cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::convert<std::string>(255, cnv(arg::base = cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::convert<std::string>(255, cnv(arg::base = cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::string_to_type(Converter const& cnv)
{
    BOOST_TEST( 255 == boost::convert<int>("255", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 999 == boost::convert<int>( "FF", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 255 == boost::convert<int>( "FF", cnv(arg::base = cnv::base::hex)).value_or(999));
    BOOST_TEST( 173 == boost::convert<int>("255", cnv(arg::base = cnv::base::oct)).value_or(999));
    BOOST_TEST( 999 != boost::convert<double>("1.23", cnv).value_or(999));
}

int
main(int argc, char const* argv[])
{
	test::sfinae();
    test::int_to_string();
    test::string_to_int();
    test::string_to_bool();
    test::string_to_type(boost::strtol_converter()); 
    test::string_to_type(boost::printf_converter());
    test::type_to_string(boost::printf_converter());
    test::user_type();
    test::algorithms();
    test::lcast_converter();
    test::callables();
    test::manipulators();
    test::locale();
    test::encryption();
    test::performance();

    return boost::report_errors();
}
