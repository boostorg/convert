// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAILS_HPP
#define BOOST_CONVERT_DETAILS_HPP

#include "../parameters.hpp"

#define CONVERTER_PARAM_FUNC(PARAM_NAME, PARAM_TYPE)    \
    this_type&                                          \
    operator()(boost::parameter::aux::tag<boost::conversion::parameter::type::PARAM_NAME, PARAM_TYPE const>::type const& arg)

#endif // BOOST_CONVERT_DETAILS_HPP
