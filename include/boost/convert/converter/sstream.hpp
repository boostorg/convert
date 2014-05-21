// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include <boost/convert/converter/base.hpp>
#include <boost/convert/detail/string_sfinae.hpp>
#include <sstream>

namespace boost 
{
    template<class Char> struct basic_stringstream_converter;

    typedef basic_stringstream_converter<char> cstringstream_converter;
    typedef basic_stringstream_converter<wchar_t> wstringstream_converter; 
}

template<class Char>
struct boost::basic_stringstream_converter
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
		!conversion::is_any_string<TypeIn>::value && conversion::is_any_string<StringOut>::value, 
		bool>::type
    operator()(TypeIn const& value_in, StringOut& result_out) const
    {
		stream_.clear();            // Clear the flags
        stream_.str(StringOut());   // Clear/empty the content of the stream 

        return !(stream_ << value_in).fail() ? (result_out = stream_.str(), true) : false;
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
		conversion::is_any_string<StringIn>::value && !conversion::is_any_string<TypeOut>::value, 
		bool>::type
    operator()(StringIn const& value_in, TypeOut& result_out) const
    {
		stream_.clear();        // Clear the flags
        stream_.str(value_in);  // Set the content of the stream 

        return !(stream_ >> result_out).fail();
    }

    this_type& operator() (std::locale const& locale) { return (stream_.imbue(locale), *this); }
    this_type& operator() (manipulator_type m) { return (m(stream_), *this); }

    template<typename Manipulator>
    this_type& operator()(Manipulator m) { return (stream_ >> m, *this); }

    CONVERTER_PARAM_FUNC(locale, std::locale)
    {
        std::locale const& locale = arg[conversion::parameter::locale];
        stream_.imbue(locale);
        return *this;
    }
    CONVERTER_PARAM_FUNC(precision, int)
    {
        int precision = arg[conversion::parameter::precision];
        stream_.precision(precision);
        return *this;
    }
    CONVERTER_PARAM_FUNC(uppercase, bool)
    {
        bool uppercase = arg[conversion::parameter::uppercase];
        uppercase ? (void) stream_.setf(std::ios::uppercase) : stream_.unsetf(std::ios::uppercase);
        return *this;
    }
    CONVERTER_PARAM_FUNC(base, boost::conversion::base::type)
    {
        conversion::base::type base = arg[conversion::parameter::base];
        
        /**/ if (base == conversion::base::dec) std::dec(stream_);
        else if (base == conversion::base::hex) std::hex(stream_);
        else if (base == conversion::base::oct) std::oct(stream_);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }
    CONVERTER_PARAM_FUNC(notation, boost::conversion::notation::type)
    {
        conversion::notation::type notation = arg[conversion::parameter::notation];
        
        /**/ if (notation == conversion::notation::     fixed)      std::fixed(stream_);
        else if (notation == conversion::notation::scientific) std::scientific(stream_);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }

    private:

    mutable stream_type stream_;
};

#endif // BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
