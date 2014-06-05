// Boost.Convert
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Many thanks to 
// *) Andrzej Krzemienski for helping great deal to partition responsibilities and to ultimately pave
//    the way for the std::tr2::optional deployment;
// *) Edward Diener the Boost Review Manager for helping with the converters' design, his continuous
//    involvement, technical and administrative help, guidance and advice;
// *) Kevlin Henney and Dave Abrahams for their ['lexical_cast]-related insights and explanations;
// *) Rob Stewart and Alex Hagen-Zanker for making sure the performance tests work as they should.
// 
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_HPP
#define BOOST_CONVERT_HPP

#include <boost/convert/detail/forward.hpp>
#include <boost/convert/detail/workarounds.hpp>
#include <boost/convert/detail/algorithm_helper.hpp>
#include <boost/convert/detail/optional.hpp>

namespace boost
{
    // C2. TypeIn is passed in to the Converter as-is.
    //     That way the converter will be able to optimize the conversion based on that TypeIn type.

    template<typename TypeOut, typename TypeIn, typename Converter>
    boost::cnv::optional<TypeOut>
    convert(TypeIn const& value_in, Converter const& converter)
    {
        try
        {
            boost::cnv::optional<TypeOut> result;

            return converter(value_in, result) ? result : boost::cnv::optional<TypeOut>();
        }
        catch (...)
        {
            return boost::cnv::optional<TypeOut>();
        }
    }

    template<typename TypeOut, typename Converter>
    typename boost::cnv::algorithm_helper<TypeOut, Converter>
    convert(Converter const& cnv)
    {
        return boost::cnv::algorithm_helper<TypeOut, Converter>(cnv);
    }
}

#endif // BOOST_CONVERT_HPP
