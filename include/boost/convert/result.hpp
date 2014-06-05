// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_RESULT_HPP
#define BOOST_CONVERT_RESULT_HPP

#include <boost/convert/forward.hpp>
#include <boost/optional.hpp>

#define USE_MINE
#ifdef USE_MINE

#include <boost/convert/detail/safebool.hpp>

namespace boost { namespace cnv
{
    // Used temporarily. To be replaced with std::tr2::optional or improved boost::optional.
    template<typename TypeOut>
    struct optional
    {
        typedef optional                  this_type;
        typedef boost::safebool<this_type> safebool;

        optional () : value_(boost::make_default<TypeOut>()), good_(false) {}

        bool                   operator! () const { return !good_; }
        operator typename safebool::type () const { return safebool(!operator!()); }

        template<typename OtherTypeOut>
        this_type& operator=(OtherTypeOut const& v) { value_ = TypeOut(v), good_ = true; return *this; }

        TypeOut& operator*() { return value_; }
        TypeOut* operator->() { return &value_; }

        TypeOut const& value() const
        {
            if (!good_)
                boost::cnv::dothrow();

            return value_;
        }

        template<typename FallbackType>
        TypeOut value_or(FallbackType const& fallback) const
        {
            return good_ ? value_ : TypeOut(fallback);
        }

        private:

        TypeOut value_;
        bool     good_;
    };
}}

#else

// Somehow the below introduces performance overhead:
// str-to-int spirit: raw/cnv=0.13/0.16 seconds.
// when our own (above) does not:
// str-to-int spirit: raw/cnv=0.13/0.13 seconds.

namespace boost { namespace cnv
{
    // Used temporarily. To be replaced with std::tr2::optional or improved boost::optional.
    template<typename TypeOut>
    struct optional : public boost::optional<TypeOut>
    {
        typedef optional                 this_type;
        typedef boost::optional<TypeOut> base_type;

        optional () {}

        template<typename OtherTypeOut>
        this_type& operator=(OtherTypeOut const& v) { base_type::operator=(TypeOut(v)); return *this; }

        TypeOut const& value() const
        {
            if (!*this)
                boost::cnv::dothrow();

            return base_type::get();
        }

        template<typename FallbackType>
        TypeOut value_or(FallbackType const& fallback) const
        {
            return !*this ? TypeOut(fallback) : base_type::get();
        }
    };
}}

#endif

#endif // BOOST_CONVERT_RESULT_HPP
