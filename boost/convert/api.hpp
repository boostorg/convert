/// \file boost/convert.hpp
/// Public interface to the Boost.Convert framework.
//
// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Special thanks to Andrzej Krzemienski who suggested a far better design,
// cleaner interface and separation of responsibilities.
// Many thanks to Robert Stewart, Scott McMurray, Andrey Semashev, Dave Abrahams,
// Hartmut Kaiser, Anders Dalvander, Andrew Troschinetz and all the Boosters
// participated in the related discussions and the review.
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

    static out_type create_storage();

    template<class TypeIn, class Converter>
    static
    result_type
    from(TypeIn const& value_in, Converter const& converter) //C1.
    {
        result_type result (this_type::create_storage()); //C2
        bool       success = converter.convert(value_in, result.value_); //C3
        
        return success ? result : result(false);
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
inline
typename boost::convert<TypeOut>::out_type
boost::convert<TypeOut>::create_storage()
{
    return out_type();
}

// Used temporarily. To be replaced with tr1::optional or improved boost::optional.
template<class TypeOut>
struct boost::convert<TypeOut>::result
{
	typedef result                 this_type;
	typedef boost::safebool<result> safebool;
    
    result (out_type const& v) : value_(v), good_(true) {}

    bool                   operator! () const { return !good_; }
    operator typename safebool::type () const { return safebool(!operator!()); }
//  operator         out_type const& () const { return this_type::value(); }

    out_type const& value() const
    {
        if (!good_)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return value_;
    }

    template<class FallbackType>
    out_type value_or(FallbackType const& fallback) const
    {
        return good_ ? value_ : fallback;
    }

    private:

	friend class boost::convert<TypeOut>;

    this_type& operator()(bool good) { return (good_ = good, *this); }

	out_type value_;
    bool      good_;
};

template<class TypeOut>
template<class TypeIn, class Converter>
struct boost::convert<TypeOut>::algorithm_helper
{
    struct with_fallback;

    typedef algorithm_helper this_type;

    algorithm_helper(Converter const& cnv) : converter_(&cnv) {}

    template<typename Arg>
    with_fallback
    operator()(parameter::aux::tagged_argument<conversion::type::fallback, Arg> const&);

    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = boost::convert<TypeOut>::create_storage();
        bool       good = converter_->convert(value_in, result);
        
        if (!good)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return result;
    }

    protected:

    Converter const* converter_;
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
        base_type   (ah),
        fallback_   (fallback)
    {}

    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = boost::convert<TypeOut>::create_storage();
        bool       good = converter_->convert(value_in, result);

        return good ? result : fallback_;
    }

    out_type fallback_;
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
