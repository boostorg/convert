/// \file boost/convert/api.hpp
/// Defines the Boost.Convert interface
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_CONVERT_PRIVATE_HPP__
#define __BOOST_CONVERT_PRIVATE_HPP__

#include "../safebool.hpp"
#include "./workarounds.hpp"
#include "./boost_parameter_ext.hpp"
#include <boost/throw_exception.hpp>
#include <boost/optional.hpp>
#include <stdexcept>

namespace boost
{
    template<class TypeOut =void, class Enable =void> struct convert;

    namespace conversion
    {
        template<class TypeOut> struct result;

        namespace parameter
        {
            BOOST_PARAMETER_KEYWORD(type,    throw_); ///< Specify throw-on-failure behavior explicitly.
            BOOST_PARAMETER_KEYWORD(type,   locale_); ///< Specify locale.
            BOOST_PARAMETER_KEYWORD(type, fallback_); ///< Provide fallback value.
            BOOST_PARAMETER_KEYWORD(type,   format_); ///< Not used. Work in progress.
        }
    }
}

/// Encapsulation of the conversion result
template<class TypeOut>
struct boost::conversion::result
{
    typedef boost::safebool<result>            safebool;
    typedef TypeOut                            out_type;
    typedef boost::optional<out_type> optional_out_type;
    enum status { good, bad, throw_bad };

    result(out_type const&          v, status s) : value_(v), status_(s) {}
    result(optional_out_type const& v, status s) : value_(v), status_(s) {}

    /// Implicit conversion to safe-bool to allow checking the success of the conversion.
    /// For example,
    /// @code
    ///     convert<foo>::result result = convert<foo>::from(str, fallback_value);
    ///     if ( result) conversion succeeded
    ///     if (!result) conversion failed
    /// @endcode
    operator typename safebool::type() const { return safebool(status_ == good); }

    /// Retrieve the actual result (value) of the conversion.
    /// For example,
    /// @code
    ///     convert<foo_type>::result result = convert<foo_type>::from(str, foo_value);
    ///     if (result)
    ///     {
    ///         foo_type value = result.value();  // Safe to retrieve the result
    ///     }
    /// @endcode
    out_type const& value() const
    {
        if (!value_ || status_ == throw_bad)
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

        return value_.get();
    }

    private:

    optional_out_type value_;
    status           status_;
};

/// Boost.Convert framework public interface
template<class TypeOut, class EnableOut>
struct boost::convert
{
    struct                              format;
    typedef conversion::result<TypeOut> result;

    /// Forward declaration of TypeIn-to-TypeOut converter.
    /// This class is implemented/specialized for every TypeIn/TypeOut combination.
    template<class TypeIn, class EnableIn =void> struct converter;

    /// Conversion API with syntax and behavior similar to boost::lexical_cast,
    /// i.e. it throws (std::invalid_argument) if conversion fails.
    /// TODO. Need to pass boost::optional<TypeOut> instead of constructing
    /// with the default constructors.
    template<class TypeIn>
    static
    converter<TypeIn>
    from(TypeIn const& value_in)
    {
        return converter<TypeIn>(value_in, TypeOut(), true);
    }

    /// Returns the 'fallback_value' if conversion fails.
    /// Throws nothing by itself.
    template<class TypeIn, class FallbackType>
    static
    converter<TypeIn>
    from(TypeIn const& value_in, FallbackType const& fallback_value)
    {
        return converter<TypeIn>(value_in, fallback_value, false);
    }

    /// Create an empty converter. To be used with algorithms or directly.
    /// TODO. Need to pass boost::optional<TypeIn> and boost::optional<TypeOut> instead of
    /// constructing with the default constructors.
    template<class TypeIn>
    static
    converter<TypeIn>
    from()
    {
        return converter<TypeIn>(TypeIn(), TypeOut(), true);
    }
};

/// Work in progress
template<class TypeOut, class Enable>
struct boost::convert<TypeOut, Enable>::format
{
    // Work in progress
};

#endif // __BOOST_CONVERT_PRIVATE_HPP__
