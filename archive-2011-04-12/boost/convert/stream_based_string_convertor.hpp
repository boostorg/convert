// Boost.Convert library
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_CONVERT_STREAM_BASED_STRING_CONVERTOR_PRIVATE_HPP__
#define __BOOST_CONVERT_STREAM_BASED_STRING_CONVERTOR_PRIVATE_HPP__

#include "./api.hpp"
#include "./string_sfinae.hpp"
#include <boost/shared_ptr.hpp>
#include <sstream>

namespace boost { namespace convert_detail {

template<typename TypeIn, typename TypeOut, typename Enable =void>
struct stream_based_string_convertor;

template<typename TypeIn, typename TypeOut>
struct stream_based_string_convertor<
    TypeIn,
    TypeOut,
    typename boost::enable_if_c<is_any_string<TypeIn>::value || is_any_string<TypeOut>::value, void>::type>
{
    typedef typename mpl::if_<is_any_string<TypeOut>,
        TypeOut, TypeIn>::type                    string_type;
    typedef typename range_value<string_type>::type char_type;
    typedef std::basic_stringstream<char_type>    stream_type;
    typedef boost::shared_ptr<stream_type> shared_stream_type; // std::streams are not copyable. Can we do better?
    typedef stream_based_string_convertor           this_type;

    typedef typename corrected_type< TypeIn>::type  corrected_in_type;
    typedef typename corrected_type<TypeOut>::type corrected_out_type;
    typedef typename corrected_type<TypeOut>::out_type       out_type;
    typedef conversion::result<out_type>                  result_type;
    typedef typename result_type::status                  status_type;
    typedef boost::optional<out_type>               optional_out_type;

    template<typename SomeTypeIn, typename FallbackTypeOut>
    stream_based_string_convertor( // For type-to-string conversions
        SomeTypeIn const& value_in,
        FallbackTypeOut const& fallback_value,
        bool dothrow,
        typename boost::enable_if_c<boost::is_same<TypeIn, SomeTypeIn>::value && !is_any_string<TypeIn>::value, void>::type* =0)
    :
        value_in_ (value_in),
        fallback_ (fallback_value),
        throw_    (dothrow ? result_type::throw_bad : result_type::bad),
        stream_   (new stream_type(std::ios_base::in | std::ios_base::out))
    {}

    template<typename SomeTypeIn, typename FallbackTypeOut>
    stream_based_string_convertor( // For string-to-type conversions
        SomeTypeIn const& value_in,
        FallbackTypeOut const& fallback_value,
        bool dothrow,
        typename boost::enable_if<is_same_string<TypeIn, SomeTypeIn>, void>::type* =0)
    :
        value_in_ (value_in),
        fallback_ (fallback_value),
        throw_    (dothrow ? result_type::throw_bad : result_type::bad),
        stream_   (new stream_type(std::ios_base::in | std::ios_base::out))
    {
        *stream_ << value_in_;
    }

    // To be used with algorithms

    template<typename SomeTypeIn>
    typename boost::enable_if_c<boost::is_same<TypeIn, SomeTypeIn>::value && !is_any_string<TypeIn>::value, this_type>::type&
    operator()(SomeTypeIn const& value_in) // For type-to-string conversions
    {
        stream_->clear();
        stream_->str(corrected_out_type());
        value_in_ = corrected_in_type(value_in);

        return *this;
    }
    template<typename OtherStringIn>
    typename boost::enable_if<is_same_string<TypeIn, OtherStringIn>, this_type>::type&
    operator()(OtherStringIn const& from_str) // For string-to-type conversions
    {
        stream_->clear();
        stream_->str(corrected_in_type(from_str));

        return *this;
    }

    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::throw_, Arg> const& arg)
    {
        bool const& value = arg[conversion::parameter::throw_];
        throw_ = value ? result_type::throw_bad : result_type::bad;

        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::fallback_, Arg> const& arg)
    {
        fallback_ = arg[conversion::parameter::fallback_];
        throw_    = result_type::bad; // Disable throw-on-failure behavior

        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::locale_, Arg> const& arg)
    {
        std::locale const& locale = arg[conversion::parameter::locale_];
        return (stream_->imbue(locale), *this);
    }

    template<typename Manipulator>
    this_type& operator>> (Manipulator m) { return (*stream_ >> m, *this); }

    out_type       value () const { return this->operator result_type().value(); }
    operator    out_type () const { return this->operator result_type().value(); }
    operator result_type () const
    {
        optional_out_type const& res(this->convert_<corrected_in_type>());

        return res ? result_type(res, result_type::good) : result_type(fallback_, throw_);
    }

    private:

    template<typename SomeTypeIn>
    typename boost::enable_if_c<boost::is_same<TypeIn, SomeTypeIn>::value && !is_any_string<TypeIn>::value, optional_out_type>::type
    convert_() const // For type-to-string conversions
    {
        bool success (*stream_ && *stream_ << value_in_);
        return success ? optional_out_type(stream_->str()) : optional_out_type();
    }

    template<typename SomeTypeIn>
    typename boost::enable_if_c<is_same_string<TypeIn, SomeTypeIn>::value, optional_out_type>::type
    convert_() const // For string-to-type conversions
    {
        out_type value (fallback_);
        bool   success (*stream_ && *stream_ >> value);

        return success ? optional_out_type(value) : optional_out_type();
    }

    corrected_in_type  value_in_;
    corrected_out_type fallback_;
    status_type           throw_;
    shared_stream_type   stream_;
};

}}

#endif // __BOOST_CONVERT_STREAM_BASED_STRING_CONVERTOR_PRIVATE_HPP__
