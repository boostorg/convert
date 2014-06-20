// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/string_sfinae.hpp>
#include <boost/make_default.hpp>
#include <sstream>
#include <iomanip>
#include <stdio.h>

namespace boost { namespace cnv
{
    template<class Char> struct basic_stream;

    typedef boost::cnv::basic_stream<char>    cstream;
    typedef boost::cnv::basic_stream<wchar_t> wstream;
}}

template<class Char>
struct boost::cnv::basic_stream : boost::noncopyable
{
    typedef Char                                 char_type;
    typedef boost::cnv::basic_stream<char_type>  this_type;
    typedef std::basic_stringstream<char_type> stream_type;
    typedef std::basic_istream<char_type>     istream_type;
    typedef std::basic_streambuf<char_type>    buffer_type;
    typedef std::basic_string<char_type>       string_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    struct parser_type : public buffer_type
    {
        using buffer_type::eback;
        using buffer_type::gptr;
        using buffer_type::egptr;

        parser_type(char_type const* beg, std::streamsize sz)
        {
            char_type* b = const_cast<char_type*>(beg);
            char_type* e = b + sz;

            buffer_type::setg(b, b, e);
        }
    };

    basic_stream()
    :
        stream_(std::ios_base::in | std::ios_base::out)
    {}
#if defined(BOOST_CONVERT_CXX11)
    basic_stream(this_type&& that)
    :
        stream_(std::move(that.stream_))
    {}
#endif
    template<typename TypeIn>
    typename boost::enable_if_c<!cnv::is_any_string<TypeIn>::value, void>::type
    operator()(TypeIn const& value_in, boost::optional<string_type>& string_out) const
    {
        stream_.clear();            // Clear the flags
        stream_.str(string_type()); // Clear/empty the content of the stream

        if (!(stream_ << value_in).fail())
            string_out = stream_.str();
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
        cnv::is_any_string<StringIn>::value && !cnv::is_any_string<TypeOut>::value, 
        void>::type
    operator()(StringIn const& string_in, boost::optional<TypeOut>& result_out) const
    {
        // C1. The code below the provided string_in as the buffer and, consequently, avoids
        //     the overhead associated with stream_.str(string_in) -- copying of the content
        //     into internal buffer.
        // C2. The "strbuf.gptr() != strbuf.egptr()" check replaces "istream.eof() != true"
        //     which for some reason does not work when we try converting the "true" string
        //     to "bool" with std::boolalpha set. Seems that istream state gets unsynced compared
        //     to the actual underlying buffer.

        istream_type& istream = stream_;
        buffer_type*   oldbuf = istream.rdbuf();
        char_type const*  beg = cnv::string_range<StringIn>::begin(string_in);
        std::streamsize    sz = cnv::string_range<StringIn>::size(string_in);
        parser_type    strbuf (beg, sz); //C1

        istream.rdbuf(&strbuf);
        istream.clear(); // Clear the flags

        istream >> *(result_out = boost::make_default<TypeOut>());

        if (istream.fail() || strbuf.gptr() != strbuf.egptr()/*C2*/)
            result_out = boost::none;

        istream.rdbuf(oldbuf);
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
