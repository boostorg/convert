// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include "./string_sfinae.hpp"
#include "./parameters.hpp"
#include <sstream>

namespace boost {

template<class Char>
struct basic_stringstream_converter
{
    typedef Char                                     char_type;
    typedef basic_stringstream_converter             this_type;
    typedef std::basic_stringstream<char_type>     stream_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    basic_stringstream_converter() 
    :
        stream_(std::ios_base::in | std::ios_base::out)
    {}

    template<typename StringOut, typename TypeIn>
    typename boost::enable_if_c<
		!convert_detail::is_any_string<TypeIn>::value && convert_detail::is_any_string<StringOut>::value, 
		bool>::type
    operator()(TypeIn const& value_in, StringOut& result_out) const
    {
		stream_.clear();            // Clear the flags
        stream_.str(StringOut());   // Clear/empty the content of the stream 

        return (!stream_.fail()  && !(stream_ << value_in).fail()) ? (result_out = stream_.str(), true) : false;
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
		convert_detail::is_any_string<StringIn>::value && !convert_detail::is_any_string<TypeOut>::value, 
		bool>::type
    operator()(StringIn const& value_in, TypeOut& result_out) const
    {
		stream_.clear();        // Clear the flags
        stream_.str(value_in);  // Set the content of the stream 

        return !stream_.fail() && !(stream_ >> result_out).fail();
    }

    this_type& operator() (std::locale const& locale) { return (stream_.imbue(locale), *this); }
    this_type& operator() (manipulator_type m) { return (stream_ >> m, *this); }

    template<typename Manipulator>
    this_type& operator()(Manipulator m) { return (stream_ >> m, *this); }

    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::locale, Arg> const& arg)
    {
        std::locale const& locale = arg[conversion::parameter::locale];
        stream_.imbue(locale);
        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::precision, Arg> const& arg)
    {
        int precision = arg[conversion::parameter::precision];
        stream_.precision(precision);
        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::uppercase, Arg> const& arg)
    {
        bool uppercase = arg[conversion::parameter::uppercase];
        uppercase ? stream_.setf(std::ios::uppercase) : stream_.unsetf(std::ios::uppercase);
        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::base, Arg> const& arg)
    {
        int base = arg[conversion::parameter::base];
        
        /**/ if (base == 10) stream_.setf(std::ios::dec);
        else if (base == 16) stream_.setf(std::ios::hex);
        else if (base ==  8) stream_.setf(std::ios::oct);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }
    template<typename Arg>
    this_type& operator()(parameter::aux::tagged_argument<conversion::parameter::type::notation, Arg> const& arg)
    {
        conversion::notation::type notation = arg[conversion::parameter::notation];
        
        /**/ if (notation == conversion::notation::     fixed) stream_.setf(std::ios::fixed);
        else if (notation == conversion::notation::scientific) stream_.setf(std::ios::scientific);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }

    private:

    mutable stream_type stream_;
};

typedef basic_stringstream_converter<char> cstringstream_converter;
typedef basic_stringstream_converter<wchar_t> wstringstream_converter; 

}

#endif // BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
