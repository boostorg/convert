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

#include <boost/convert/detail/checks.hpp>
#include <boost/convert/detail/algorithm_helper.hpp>

namespace boost
{
    /// @brief The main Boost.Convert deployment interface
    /// @details This is the Boost.Convert main interface. For example,
    /// @code
    ///    boost::cnv::cstringstream cnv;
    ///
    ///    int    i1 = boost::convert<int>(" 12", cnv(std::skipws)).value();
    ///    int    i2 = boost::convert<int>("uhm", cnv).value_or(-1);
    ///    string s1 = boost::convert<string>(123.456, cnv)).value();
    ///    string s2 = boost::convert<string>(123.456, cnv)).value_or("failed");
    /// @endcode

    template<typename TypeOut, typename TypeIn, typename Converter>
    boost::optional<TypeOut>
    convert(TypeIn const& value_in, Converter const& converter)
    {
            boost::optional<TypeOut> result;
            converter(value_in, result);
            return result;
    }

    /// @brief The main Boost.Convert deployment interface with algorithms
    /// @details This the Boost.Convert main deployment interface with algorithms. For example,
    /// @code
    ///    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    ///    std::vector<int>             ints;
    ///    boost::cnv::cstringstream     cnv;
    ///
    ///    cnv(std::hex)(std::skipws);
    ///
    ///    std::transform(
    ///        strings.begin(),
    ///        strings.end(),
    ///        std::back_inserter(integers),
    ///        boost::convert<int>(cnv).value_or(INT_MAX));
    /// @endcode

    template<typename TypeOut, typename Converter>
    typename boost::cnv::algorithm_helper<TypeOut, Converter>
    convert(Converter const& cnv)
    {
        return boost::cnv::algorithm_helper<TypeOut, Converter>(cnv);
    }
}

#endif // BOOST_CONVERT_HPP
