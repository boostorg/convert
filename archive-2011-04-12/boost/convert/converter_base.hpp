// Boost.Convert library
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_BASE_IMPLEMENTATION_PRIVATE_HPP__
#define __BOOST_BASE_IMPLEMENTATION_PRIVATE_HPP__

#include "./api.hpp"
#include "./string_sfinae.hpp"

namespace boost { namespace convert_detail {

template<class Derived, class TypeIn, class TypeOut>
struct converter_base
{
    typedef typename corrected_type< TypeIn>::type  corrected_in_type;
    typedef typename corrected_type<TypeOut>::type corrected_out_type;
    typedef typename corrected_type<TypeOut>::out_type       out_type;
    typedef conversion::result<out_type>                  result_type;
    typedef typename result_type::status                  status_type;
    typedef boost::optional<out_type>               optional_out_type;

    template<class FallbackTypeOut>
    converter_base(TypeIn const& value_in, FallbackTypeOut const& fallback_value, bool dothrow)
    :
        value_in_ (value_in),
        fallback_ (fallback_value),
        throw_    (dothrow ? result_type::throw_bad : result_type::bad)
    {}

    // To be used with algorithms
    template<class OtherTypeIn>
    Derived& operator()(OtherTypeIn const& value_in)
    {
        return (value_in_ = corrected_in_type(value_in), *(Derived*) this);
    }

    template<typename Arg>
    Derived& operator()(parameter::aux::tagged_argument<conversion::parameter::type::throw_, Arg> const& arg)
    {
        bool const& value = arg[conversion::parameter::throw_];
        throw_ = value ? result_type::throw_bad : result_type::bad;

        return *(Derived*) this;
    }
    template<typename Arg>
    Derived& operator()(parameter::aux::tagged_argument<conversion::parameter::type::fallback_, Arg> const& arg)
    {
        fallback_ = arg[conversion::parameter::fallback_];
        throw_    = result_type::bad; // Disable throw-on-failure behavior

        return *(Derived*) this;
    }
    out_type       value () const { return this->operator result_type().value(); }
    operator    out_type () const { return this->operator result_type().value(); }
    operator result_type () const
    {
        optional_out_type const& ret (((Derived*) this)->convert_(value_in_));

        return ret ? result_type(ret, result_type::good) : result_type(fallback_, throw_);
    }

    private:

    corrected_in_type  value_in_;
    corrected_out_type fallback_;
    status_type           throw_;
};

}}

#endif // __BOOST_BASE_IMPLEMENTATION_PRIVATE_HPP__
