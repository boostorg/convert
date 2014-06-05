// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/string_sfinae.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>

namespace boost { namespace cnv
{
    template<class Char> struct basic_stringstream;

    typedef boost::cnv::basic_stringstream<char>    cstringstream;
    typedef boost::cnv::basic_stringstream<wchar_t> wstringstream;
}}

template<class Char>
struct boost::cnv::basic_stringstream
{
    typedef Char                                                char_type;
    typedef boost::cnv::basic_stringstream<char_type>           this_type;
    typedef std::basic_stringstream<char_type>                stream_type;
    typedef std::basic_istream<char_type>                    istream_type;
    typedef std::basic_streambuf<char_type>                   buffer_type;
    typedef boost::detail::parser_buf<buffer_type, char_type> parser_type;
    typedef std::basic_string<char_type>                      string_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    basic_stringstream()
    :
        stream_(std::ios_base::in | std::ios_base::out)
    {}

    template<typename TypeIn>
    typename boost::enable_if_c<!cnv::is_any_string<TypeIn>::value, bool>::type
    operator()(TypeIn const& value_in, boost::cnv::optional<string_type>& string_out) const
    {
        stream_.clear();            // Clear the flags
        stream_.str(string_type()); // Clear/empty the content of the stream

        return !(stream_ << value_in).fail() ? (string_out = stream_.str(), true) : false;
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
        cnv::is_any_string<StringIn>::value && !cnv::is_any_string<TypeOut>::value, 
        bool>::type
    operator()(StringIn const& string_in, boost::cnv::optional<TypeOut>& result_out) const
    {
        typedef cnv::string_range<StringIn> str_range;

        istream_type& istream = stream_;
        parser_type    strbuf;
        buffer_type*   oldbuf = istream.rdbuf();
        char_type const*  beg = str_range::begin(string_in);
        std::streamsize    sz = str_range::size(string_in);

        stream_.clear(); // Clear the flags

        // The code below (pretty much stolen from shr_using_base_class(InputStreamable& output) in lexical_cast.hpp
        // uses the provided string_in as the buffer and, consequently, avoids the overhead associated with
        // stream_.str(string_in) -- copying of the content into internal buffer.

        strbuf.setbuf(const_cast<char_type*>(beg), sz);
        istream.rdbuf(&strbuf);
        istream >> *(result_out = boost::make_default<TypeOut>());
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
        std::locale const& locale = arg[cnv::parameter::locale];
        stream_.imbue(locale);
        return *this;
    }
    CONVERTER_PARAM_FUNC(precision, int)
    {
        int precision = arg[cnv::parameter::precision];
        stream_.precision(precision);
        return *this;
    }
    CONVERTER_PARAM_FUNC(uppercase, bool)
    {
        bool uppercase = arg[cnv::parameter::uppercase];
        uppercase ? (void) stream_.setf(std::ios::uppercase) : stream_.unsetf(std::ios::uppercase);
        return *this;
    }
    CONVERTER_PARAM_FUNC(base, boost::cnv::base::type)
    {
        cnv::base::type base = arg[cnv::parameter::base];
        
        /**/ if (base == cnv::base::dec) std::dec(stream_);
        else if (base == cnv::base::hex) std::hex(stream_);
        else if (base == cnv::base::oct) std::oct(stream_);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }
    CONVERTER_PARAM_FUNC(notation, boost::cnv::notation::type)
    {
        cnv::notation::type notation = arg[cnv::parameter::notation];
        
        /**/ if (notation == cnv::notation::     fixed)      std::fixed(stream_);
        else if (notation == cnv::notation::scientific) std::scientific(stream_);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }

    private:

    mutable stream_type stream_;
};

#endif // BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
