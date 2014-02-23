// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CONVERTER_BASE_HPP
#define BOOST_CONVERT_CONVERTER_BASE_HPP

#include "../details.hpp"

namespace boost 
{
    struct converter_base;
}

struct boost::converter_base
{
    typedef boost::converter_base this_type;

    converter_base()
    :
        base_(10)
    {}

    this_type&
    operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg)
    {
        boost::conversion::base::type base = arg[conversion::parameter::base];
        
        base_ = base == conversion::base::dec ? 10
              : base == conversion::base::hex ? 16
              : base == conversion::base::oct ? 8 : 0;

        return *this;
    }
    
    protected:

    int base_;
};

#endif // BOOST_CONVERT_CONVERTER_BASE_HPP
