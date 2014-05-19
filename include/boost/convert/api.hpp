// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Many thanks to 
// *) Andrzej Krzemienski for helping great deal to partition responsibilities and to ultimately pave
//    the way for the tr1::optional deployment;
// *) Edward Diener the Boost Review Manager for helping with the converters' design, his continuous
//    involvement, technical and administrative help, guidance and advice;
// *) Kevlin Henney and Dave Abrahams for their ['lexical_cast]-related insights and explanations;
// *) Rob Stewart and Alex Hagen-Zanker for making sure the performance tests work as they should.
// 
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_API_HPP
#define BOOST_CONVERT_API_HPP

#include <boost/convert/detail/safebool.hpp>
#include <boost/convert/detail/workarounds.hpp>
#include <boost/convert/detail/string.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost
{
    template<typename> struct convert;
}

template<typename TypeOut>
struct boost::convert
{
	// C1. TypeOut needs to be normalized as, say, "char const*" might be provided when
	//     std::string will be used instead (as we have to have storage for the conversion result).
	// C2. TypeIn on the other hand needs to be passed in to the Converter as-is.
	//     That way the converter will be able to optimize the conversion based on that TypeIn type.
	// C3. convert::from() allocates storage for the conversion result.
	//     The Pascal-style passing of the out_type& to the converter is ugly. However, it
	//     a) ensures the consistent requirement with regard to "out_type" 
	//        (rather than every converter imposing their own);
	//     b) relieves the converter of that responsibility and makes writing converters easier.
	
	struct result;
    template<typename, typename> struct algorithm_helper;

	typedef boost::convert<TypeOut>	                         this_type;
	typedef typename convert_detail::corrected<TypeOut>::type out_type; //C1
	typedef typename boost::convert<out_type>::result      result_type; //C1

    static out_type create_storage() { return out_type(); }

    template<typename TypeIn, typename Converter>
    static
    result_type
    from(TypeIn const& value_in, Converter const& converter)
    {
        result_type result (this_type::create_storage()); //C1,C3
        bool       success = converter(value_in, result.value_); //C1,C2,C3
        
        return success ? result : result(false);
    }

    template<typename TypeIn, typename Converter>
    static
    algorithm_helper<TypeIn, Converter>
    from(Converter const& cnv)
    {
        return algorithm_helper<TypeIn, Converter>(cnv);
    }
};

// Used temporarily. To be replaced with tr1::optional or improved boost::optional.
template<typename TypeOut>
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

    template<typename FallbackType>
    out_type value_or(FallbackType const& fallback) const
    {
        return good_ ? value_ : fallback;
    }

    private:

	friend struct boost::convert<TypeOut>;
    template<typename, typename> friend struct boost::convert<TypeOut>::algorithm_helper;

    this_type& operator()(bool good) { return (good_ = good, *this); }

	out_type value_;
    bool      good_;
};

template<typename TypeOut>
template<typename TypeIn, typename Converter>
struct boost::convert<TypeOut>::algorithm_helper
{
    struct with_fallback;

    typedef algorithm_helper this_type;

    algorithm_helper(Converter const& cnv) : converter_(&cnv) {}

    template<typename FallbackType>
    with_fallback
    value_or(FallbackType const&);

    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = boost::convert<TypeOut>::create_storage();
        bool       good = (*converter_)(value_in, result);

        if (!good)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return result;
    }

    protected:

    Converter const* converter_;
};

template<typename TypeOut>
template<typename TypeIn, typename Converter>
struct boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>::with_fallback
:
    public boost::convert<TypeOut>::template algorithm_helper<TypeIn, Converter>
{
    typedef with_fallback                                                this_type;
#if defined(_MSC_VER)
    typedef typename boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter> base_type;
#else
    typedef boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter> base_type;
#endif
    with_fallback(base_type const& ah, TypeOut const& fallback)
    :
        base_type   (ah),
        fallback_   (fallback)
    {}

    TypeOut operator()(TypeIn const& value_in)
    {
        out_type result = boost::convert<TypeOut>::create_storage();
        bool       good = (*converter_)(value_in, result);

        return good ? result : fallback_;
    }

    out_type fallback_;
};

template<typename TypeOut>
template<typename TypeIn, typename Converter>
template<typename FallbackType>
typename boost::convert<TypeOut>::template algorithm_helper<TypeIn, Converter>::with_fallback
boost::convert<TypeOut>::algorithm_helper<TypeIn, Converter>::value_or(FallbackType const& fallback)
{
    return with_fallback(*this, fallback);
}

#endif // BOOST_CONVERT_API_HPP
