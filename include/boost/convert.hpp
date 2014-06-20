// Boost.Convert
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Many thanks to Julian Gonggrijp, Rob Stewart, Andrzej Krzemienski, Matus Chochlik, Jeroen Habraken,
// Hartmut Kaiser, Joel De Guzman, Thijs (M.A.) van den Berg, Roland Bock, Gavin Lambert, Paul Bristow,
// Alex Hagen-Zanker, Christopher Kormanyos for taking part in the Boost.Convert review.
//
// Special thanks to:
//
// 1. Alex Hagen-Zanker, Roland Bock, Rob Stewart for their considerable contributions to the design
//    and implementation of the library;
// 2. Andrzej Krzemienski for helping great deal to partition responsibilities and to ultimately pave
//    the way for the boost::optional and future std::tr2::optional deployment;
// 3. Edward Diener the Boost Review Manager for helping with the converters' design, his continuous
//    involvement, technical and administrative help, guidance and advice;
// 4. Joel De Guzman, Rob Stewart and Alex Hagen-Zanker for making sure the performance tests work
//    as they should;
// 5. Paul Bristow for helping great deal with the documentation;
// 6. Kevlin Henney and Dave Abrahams for their lexical_cast-related insights and explanations.
// 
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_HPP
#define BOOST_CONVERT_HPP

#include <boost/convert/detail/is.hpp>

namespace boost
{
    struct throw_on_failure_type { enum type {};};
    throw_on_failure_type::type const throw_on_failure = throw_on_failure_type::type(0);

    namespace cnv
    {
        template<typename, typename, typename> struct adapter;
    }

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
    typename enable_if<cnv::is_cnv<Converter, TypeIn, TypeOut>, optional<TypeOut> >::type
    convert(TypeIn const& value_in, Converter const& converter)
    {
            optional<TypeOut> result;
            converter(value_in, result);
            return result;
    }

    template<typename TypeOut, typename TypeIn, typename Converter>
    TypeOut
    convert(TypeIn const& value_in, Converter const& converter, throw_on_failure_type::type)
    {
        return convert<TypeOut>(value_in, converter).value();
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename FallbackValue>
    typename enable_if<is_convertible<FallbackValue, TypeOut>, TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, FallbackValue const& fallback)
    {
        return convert<TypeOut>(value_in, converter).value_or(fallback);
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename FallbackFunc>
    typename enable_if<cnv::is_fun<FallbackFunc, TypeOut>, TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, FallbackFunc fallback)
    {
        return convert<TypeOut>(value_in, converter).value_or_eval(fallback);
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

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename enable_if<cnv::is_cnv<Converter, TypeIn, TypeOut>,
    typename cnv::adapter<TypeOut, TypeIn, Converter> >::type
#ifdef BOOST_CONVERT_CXX11
    convert(Converter&& cnv)
    {
        return cnv::adapter<TypeOut, TypeIn, Converter>(std::forward<Converter>(cnv));
    }
#else
    convert(Converter const& cnv)
    {
        return cnv::adapter<TypeOut, TypeIn, Converter>(cnv);
    }
#endif
}

namespace boost { namespace cnv
{
    template<typename TypeOut, typename TypeIn, typename Converter>
    struct adapter
    {
        typedef adapter this_type;

#ifdef BOOST_CONVERT_CXX11
        adapter(Converter&& cnv) : converter_(std::forward<Converter>(cnv)) {}
#else
        adapter(Converter const& cnv) : converter_(cnv) {}
#endif
        this_type&
        value_or(TypeOut const& fallback)
        {
            return (fallback_ = fallback, *this);
        }

        TypeOut operator()(TypeIn const& value_in)
        {
            optional<TypeOut> result = convert<TypeOut>(value_in, unwrap_ref(converter_));
            return fallback_ ? result.value_or(*fallback_) : result.value();
        }

        protected:

        Converter        converter_;
        optional<TypeOut> fallback_;
    };
}}

#endif // BOOST_CONVERT_HPP
