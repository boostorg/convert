// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CONVERTER_BASE_HPP
#define BOOST_CONVERT_CONVERTER_BASE_HPP

#include "../parameters.hpp"

namespace boost 
{
    struct converter_base;
}

struct boost::converter_base
{
    typedef boost::converter_base this_type;

    converter_base()
    :
        base_(10), precision_(0), uppercase_(false)
    {}

    void operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg) const
    {
        boost::conversion::base::type base = arg[conversion::parameter::base];
        
        base_ = base == conversion::base::dec ? 10
              : base == conversion::base::hex ? 16
              : base == conversion::base::oct ? 8 : 0;
    }
    void operator()(parameter::aux::tag<conversion::parameter::type::precision, int const>::type const& arg) const
    {
        precision_ = arg[conversion::parameter::precision];
    }
    void operator()(parameter::aux::tag<conversion::parameter::type::uppercase, bool const>::type const& arg) const
    {
        uppercase_ = arg[conversion::parameter::uppercase];
    }
    
    protected:

    int mutable       base_;
    int mutable  precision_;
    bool mutable uppercase_;
};

#define CONVERT_FUNC_SET_BASE                                                                                               \
    this_type const&                                                                                                        \
    operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg) const \
    { return (base_type::operator()(arg), *this);  }

#define CONVERT_FUNC_SET_PRECISION                                                                                          \
    this_type const&                                                                                                        \
    operator()(parameter::aux::tag<conversion::parameter::type::precision, int const>::type const& arg) const               \
    { return (base_type::operator()(arg), *this);  }

#define CONVERT_FUNC_SET_UPPERCASE                                                                                          \
    this_type const&                                                                                                        \
    operator()(parameter::aux::tag<conversion::parameter::type::uppercase, bool const>::type const& arg) const              \
    { return (base_type::operator()(arg), *this);  }

#endif // BOOST_CONVERT_CONVERTER_BASE_HPP
