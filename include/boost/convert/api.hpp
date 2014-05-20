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

#include <boost/convert/detail/workarounds.hpp>
#include <boost/convert/detail/algorithm_helper.hpp>
#include <boost/convert/result.hpp>

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
	
	typedef boost::convert<TypeOut>	                         this_type;
	typedef typename convert_detail::corrected<TypeOut>::type out_type; //C1
	typedef typename boost::conversion::result<out_type>   result_type; //C1

    template<typename TypeIn, typename Converter>
    static
    result_type
    from(TypeIn const& value_in, Converter const& converter)
    {
        result_type result (boost::allocate_storage<out_type>()); //C1,C3
        bool       success = converter(value_in, result.value_); //C1,C2,C3
        
        return success ? result : result(false);
    }
};

namespace boost
{
    template<typename TypeOut, typename TypeIn, typename Converter>
    boost::conversion::result<TypeOut>
    cnv(TypeIn const& value_in, Converter const& converter)
    {
        return boost::convert<TypeOut>::from(value_in, converter);
    }

    template<typename TypeOut, typename Converter>
    typename boost::conversion::algorithm_helper<TypeOut, Converter>
    cnv(Converter const& cnv)
    {
        return boost::conversion::algorithm_helper<TypeOut, Converter>(cnv);
    }
}

#endif // BOOST_CONVERT_API_HPP
