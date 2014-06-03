// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_RESULT_HPP
#define BOOST_CONVERT_RESULT_HPP

#include <boost/convert/forward.hpp>
#include <boost/convert/detail/safebool.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost { namespace conversion
{
    // Used temporarily. To be replaced with std::tr2::optional or improved boost::optional.
    template<typename TypeOut>
    struct optional
    {
        typedef optional                  this_type;
        typedef boost::safebool<this_type> safebool;

        optional (TypeOut const& v) : value_(v), good_(true) {}

        bool                   operator! () const { return !good_; }
        operator typename safebool::type () const { return safebool(!operator!()); }
    //  operator          TypeOut const& () const { return this_type::value(); }

        TypeOut const& value() const
        {
            if (!good_)
                BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

            return value_;
        }

        template<typename FallbackType>
        TypeOut value_or(FallbackType const& fallback) const
        {
            return good_ ? value_ : fallback;
        }

        private:

        template<typename Type_Out, typename TypeIn, typename Converter>
        friend
        boost::conversion::optional<Type_Out>
        boost::convert(TypeIn const& value_in, Converter const& converter);

        this_type& operator()(bool good) { return (good_ = good, *this); }

        TypeOut value_;
        bool     good_;
    };
}}

#endif // BOOST_CONVERT_RESULT_HPP
