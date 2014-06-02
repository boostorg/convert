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
    typedef Char                                         char_type;
    typedef basic_stringstream_converter                 this_type;
    typedef std::basic_stringstream<char_type>         stream_type;
    typedef std::basic_istream<char_type>             istream_type;
    typedef std::basic_streambuf<char_type>            buffer_type;
    typedef detail::parser_buf<buffer_type, char_type> parser_type;
    typedef std::basic_string<char_type>               string_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    basic_stringstream_converter() 
    :
        stream_(std::ios_base::in | std::ios_base::out)
    {}

    template<typename TypeIn>
    typename boost::enable_if_c<!conversion::is_any_string<TypeIn>::value, bool>::type
    operator()(TypeIn const& value_in, string_type& string_out) const
    {
        stream_.clear();            // Clear the flags
        stream_.str(string_type()); // Clear/empty the content of the stream

        return !(stream_ << value_in).fail() ? (string_out = stream_.str(), true) : false;
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
        conversion::is_any_string<StringIn>::value && !conversion::is_any_string<TypeOut>::value, 
        bool>::type
    operator()(StringIn const& string_in, TypeOut& result_out) const
    {
        typedef conversion::string_range<StringIn> str_range;

        istream_type& istream = stream_;
        parser_type    strbuf;
        buffer_type*   oldbuf = istream.rdbuf();
        char_type const*  beg = str_range::begin(string_in);
        std::streamsize    sz = str_range::size(string_in);

        stream_.clear();        // Clear the flags
//      stream_.str(string_in); // Copy the content to the internal buffer
//      stream_ >> result_out;

        // The code below (pretty much stolen from shr_using_base_class(InputStreamable& output) in lexical_cast.hpp
        // uses the provided string_in as the buffer and, consequently, avoids the overhead associated with
        // stream_.str(string_in) -- copying of the content into internal buffer.

        strbuf.setbuf(const_cast<char_type*>(beg), sz);
        istream.rdbuf(&strbuf);
        istream >> result_out;
        bool result = !istream.fail() && istream.eof();
        istream.rdbuf(oldbuf);

        return result;
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
