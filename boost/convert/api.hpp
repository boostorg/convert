/// \file boost/convert.hpp
/// Public interface to the Boost.Convert framework.
//
// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Many thanks to Robert Stewart, Scott McMurray, Andrey Semashev, Dave Abrahams,
// Hartmut Kaiser, Anders Dalvander, Andrzej Krzemienski, Andrew Troschinetz and
// all the Boosters participated in the related discussions and the review.
// In many ways the library has been influenced and shaped by them.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_API_HPP
#define BOOST_CONVERT_API_HPP

#include "./safebool.hpp"
#include "./workarounds.hpp"
#include "./string_sfinae.hpp"
#include "./parameters.hpp"
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost
{
    template<class> struct convert;
}

template<class TypeOut>
struct boost::convert
{
    // C1. Conversion API with syntax and behavior similar to boost::lexical_cast.
    //     Namely, if conversion fails, it (eventually in result.value()) throws std::invalid_argument.
    // C2. When no fallback value is provided. out_type needs to be default-constructible (as for boost::lexical_cast).    
    //     When a fallback value is provided, the temporary holding the conversion result is copy-constructed and,
    //     therefore, out_type does not need to be default-constructible.
    // C3. The ugly way of passing the return argument as a parameter is
    //     to relieve the converter of the need to construct an instance of out_type.
    //     That way we avoid the default-constructibility requirement when it can be avoided.

    struct                                  result;
    template<class, class> struct algorithm_helper;

	typedef boost::convert<TypeOut>	                         this_type;
	typedef typename convert_detail::corrected<TypeOut>::type out_type;
	typedef typename this_type::result                     result_type;
	typedef typename result_type::status                   status_type;

    template<class TypeIn, class Converter>
    static
    result_type
    from(TypeIn const& value_in, Converter const& converter) //C1.
    {
        result_type result (out_type(), result_type::good); //C2
        bool       success = converter.convert(value_in, result.value_); //C3
        
        return success ? result : result(result_type::throw_bad);
    }
    template<class TypeIn, class FallbackType, class Converter>
    static
    result_type
    from(TypeIn const& value_in, FallbackType const& fallback, Converter const& converter)
    {
        result_type result (fallback, result_type::good); //C2
        bool       success = converter.convert(value_in, result.value_); //C3
        
        return success ? result : result_type(fallback, result_type::bad); // Do not expect result.value_=fallback when failed
    }
    template<class TypeIn, class Converter>
    static
    algorithm_helper<TypeIn, Converter>
    from(Converter const& cnv)
    {
        return algorithm_helper<TypeIn, Converter>(cnv);
    }
};

template<class TypeOut>
struct boost::convert<TypeOut>::result
{
	typedef result                 this_type;
	typedef boost::safebool<result> safebool;
    
    enum status { good, bad, throw_bad };

    result (out_type const& v, status s) : value_(v), status_(s) {}

    bool                   operator! () const { return status_ != good; }
    operator typename safebool::type () const { return safebool(!operator!()); }
//  operator         out_type const& () const { return this_type::value(); }

    out_type const& value() const
    {
        if (status_ == throw_bad)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return value_;
    }

    template<class FallbackType>
    out_type const& value_or(FallbackType const& fallback) const
    {
        return status_ == good ? value_ : fallback;
    }

    this_type&
    operator()(conversion::throw_on_failure_t)
    {
		if (status_ == result_type::bad) status_ = result_type::throw_bad;

        return *this;
    }

    private:

	friend class boost::convert<TypeOut>;

    this_type& operator()(this_type::status status) { return (status_ = status, *this); }

	out_type     value_;
    status_type status_;
};

template<class TypeOut>
template<class TypeIn, class Converter>
struct boost::convert<TypeOut>::algorithm_helper
{
    struct with_fallback;

    typedef algorithm_helper this_type;

    algorithm_helper(Converter const& cnv) : converter_(&cnv), status_(result_type::throw_bad) {}

    template<typename Arg>
    with_fallback
    operator()(parameter::aux::tagged_argument<conversion::type::fallback, Arg> const&);

    this_type& 
    operator()(conversion::throw_on_failure_t)
    {
        if (status_ == result_type::bad) status_ = result_type::throw_bad;

        return *this;
    }
    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = out_type();
        bool    success = converter_->convert(value_in, result);
        
        return success ? result : result_type(out_type(), status_).value(/*might throw here*/);
    }

    protected:

    Converter const* converter_;
    status_type         status_;
};

template<class TypeOut>
template<class TypeIn, class Converter>
struct boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>::with_fallback
:
    public boost::convert<TypeOut>::template algorithm_helper<TypeIn, Converter>
{
    typedef boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>::with_fallback this_type;
    typedef boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>                base_type;

    with_fallback(base_type const& ah, TypeOut const& fallback)
    :
        base_type(ah),
        value_(fallback)
    {
        base_type::status_ = result_type::bad; // Disable throw-on-failure behavior
    }

    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = value_;
        bool    success = base_type::converter_->convert(value_in, result);

        return success ? result : result_type(value_, base_type::status_).value(/*might throw here*/);
    }
    out_type value_;
};

template<class TypeOut>
template<class TypeIn, class Converter>
template<typename Arg>
typename boost::convert<TypeOut>::template algorithm_helper<TypeIn, Converter>::with_fallback
boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>::operator()(parameter::aux::tagged_argument<conversion::type::fallback, Arg> const& arg)
{
    return with_fallback(*this, arg[conversion::fallback]);
}

#endif // BOOST_CONVERT_API_HPP
