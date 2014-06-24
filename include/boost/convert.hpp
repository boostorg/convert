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
// 2. Andrzej Krzemienski for helping to partition responsibilities and to ultimately pave
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
    namespace detail
    {
        enum throw_on_failure {};
    }
    /// @details The boost::throw_on_failure is the name of an object of the boost::detail::throw_on_failure
    /// type that is used for disambiguation of desired exception-throwing behavior.
    detail::throw_on_failure const throw_on_failure = detail::throw_on_failure(0);
}

namespace boost
{
    namespace cnv
    {
        template<typename, typename, typename> struct adapter;
        struct by_default;
    }
}

namespace boost
{
    // C1. The "is_cnv<Converter>" check is done twice -- in the main convert(TypeIn, Convert) and in the
    //     secondary-interface (derived from the main) functions as well. Strictly speaking, the checks are
    //     not necessary in the derived functions as the main convert() does the check anyway.
    //     However, when things go wrong, the error messages seem considerably clearer/friendlier with that
    //     "additional" check as the error message points to the actual line in the application(!) code where
    //     the derived API was called incorrectly. Without that "additional" check the compiler points to the
    //     call to the main convert() inside the derived function.

    /// @brief The main Boost.Convert deployment interface
    /// @param[in] value_in   The value to be converted
    /// @param[in] converter  The converter to be used for conversion
    /// @return The result of conversion together with the indication of success or failure
    /// of the conversion request.
    /// @details For example,
    /// @code
    ///    boost::cnv::cstream cnv;
    ///
    ///    boost::optional<int>    i = boost::convert<int>("12", cnv);
    ///    boost::optional<string> s = boost::convert<string>(123.456, cnv);
    /// @endcode

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename enable_if<cnv::is_cnv<typename unwrap_reference<Converter>::type, TypeIn, TypeOut>, optional<TypeOut> >::type
    convert(TypeIn const& value_in, Converter const& converter)
    {
        optional<TypeOut> result;
        boost::unwrap_ref(converter)(value_in, result);
        return result;
    }

    /// @brief The Boost.Convert deployment interface with the default converter
    /// @details For example,
    /// @code
    ///    // boost::cnv::lexical_cast (through boost::cnv::by_default) will be deployed as
    ///    // the default converter when no converter is provided explicitly.
    ///
    ///    struct boost::cnv::by_default : public boost::cnv::lexical_cast {};
    ///
    ///    boost::cnv::cstream cnv;
    ///
    ///    boost::optional<int>    i = boost::convert<int>("12");
    ///    boost::optional<string> s = boost::convert<string>(123.456);
    /// @endcode

    template<typename TypeOut, typename TypeIn>
    optional<TypeOut>
    convert(TypeIn const& value_in)
    {
        return boost::convert<TypeOut>(value_in, boost::cnv::by_default());
    }

    template<typename TypeOut, typename TypeIn, typename Converter>
    typename enable_if<cnv::is_cnv<Converter, TypeIn, TypeOut>, TypeOut>::type //See C1
    convert(TypeIn const& value_in, Converter const& converter, boost::detail::throw_on_failure)
    {
        return convert<TypeOut>(value_in, converter).value();
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename Fallback>
    typename enable_if_c<is_convertible<Fallback, TypeOut>::value &&
                         cnv::is_cnv<Converter, TypeIn, TypeOut>::value, //See C1
        TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, Fallback const& fallback)
    {
        return convert<TypeOut>(value_in, converter).value_or(fallback);
    }

    template<typename TypeOut, typename TypeIn, typename Converter, typename Fallback>
    typename enable_if_c<cnv::is_fun<Fallback, TypeOut>::value &&
                         cnv::is_cnv<Converter, TypeIn, TypeOut>::value, //See C1
        TypeOut>::type
    convert(TypeIn const& value_in, Converter const& converter, Fallback fallback)
    {
        return convert<TypeOut>(value_in, converter).value_or_eval(fallback);
    }

    /// @brief The main Boost.Convert deployment interface with algorithms
    /// @details This the Boost.Convert main deployment interface with algorithms. For example,
    /// @code
    ///    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    ///    std::vector<int>             ints;
    ///    boost::cnv::cstream           cnv;
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
