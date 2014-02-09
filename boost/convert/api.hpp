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
#include <boost/optional.hpp>
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
    // C3. The ugly way of passing the return argument as a parameter to the
    //     converter is to relieve the converter of the need to construct an
    //     instance of out_type. That way we avoid the default-constructibility
    //     requirement when it can be avoided.

    struct                                  result;
    template<class, class> struct algorithm_helper;

	typedef boost::convert<TypeOut>	                         this_type;
	typedef typename convert_detail::corrected<TypeOut>::type out_type;
	typedef boost::optional<out_type>                optional_out_type;
	typedef typename this_type::result                     result_type;
	typedef typename result_type::status                   status_type;

    template<class TypeIn, class Converter>
    static
    result_type
    from(TypeIn const& value_in, Converter const& converter) //C1.
    {
        out_type result = out_type(); //C2
        bool    success = converter.convert(value_in, result); //C3
        
        return success ? result_type(result, result_type::good) : result_type();
    }
    template<class TypeIn, class FallbackType, class Converter>
    static
    result_type
    from(TypeIn const& value_in, FallbackType const& fallback, Converter const& converter)
    {
        out_type result = out_type(fallback); //C2
        bool    success = converter.convert(value_in, result); //C3
        
        return success ? result_type(result, result_type::good) : result_type(fallback, result_type::bad);
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

    result () : status_(throw_bad) {}
    result (out_type const& v, status s) : value_(v), status_(s) {}

    operator typename safebool::type () const { return safebool(!operator!()); }
    bool                   operator! () const { return status_ != good; }

    out_type const& value() const
    {
        if (!value_ || status_ == throw_bad)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return value_.get();
    }
	operator out_type const&() const
	{ 
		return this_type::value(); 
	}
    template<typename Arg>
    this_type& 
    operator()(parameter::aux::tagged_argument<conversion::parameter::type::throw_on_failure, Arg> const& arg)
    {
        bool const& value = arg[conversion::parameter::throw_on_failure];
        
		/**/ if (status_ == result_type::bad && value) status_ = result_type::throw_bad;
		else if (status_ == result_type::throw_bad && !value) status_ = result_type::bad;

        return *this;
    }

    private:

    optional_out_type value_;
    status           status_;
};

template<class TypeOut>
template<class TypeIn, class Converter>
struct boost::convert<TypeOut>::algorithm_helper
{
	typedef algorithm_helper this_type;

    algorithm_helper(Converter const& cnv) : converter_(&cnv), throw_(result_type::throw_bad) {}

    template<typename Arg>
    this_type& 
    operator()(parameter::aux::tagged_argument<conversion::parameter::type::fallback, Arg> const& arg)
    {
        fallback_ = arg[conversion::parameter::fallback];
        throw_    = result_type::bad; // Disable throw-on-failure behavior

        return *this;
    }
    template<typename Arg>
    this_type& 
    operator()(parameter::aux::tagged_argument<conversion::parameter::type::throw_on_failure, Arg> const& arg)
    {
        bool const& value = arg[conversion::parameter::throw_on_failure];
        
		/**/ if (throw_ == result_type::bad && value) throw_ = result_type::throw_bad;
		else if (throw_ == result_type::throw_bad && !value) throw_ = result_type::bad;

        return *this;
    }
    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = fallback_.get_value_or(out_type());
        bool    success = converter_->convert(value_in, result);
        
        return success ? result : fallback_ ? *fallback_ : result_type(out_type(), throw_).value(/*might throw here*/);
    }

    Converter const* converter_;
    optional_out_type fallback_;
    status_type          throw_;
};

#endif // BOOST_CONVERT_API_HPP
