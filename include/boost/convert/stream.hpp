// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/is_char.hpp>
#include <boost/make_default.hpp>
#include <sstream>
#include <iomanip>

namespace boost { namespace cnv
{
    template<class Char> struct basic_stream;

    typedef boost::cnv::basic_stream<char>    cstream;
    typedef boost::cnv::basic_stream<wchar_t> wstream;
}}

template<class Char>
struct boost::cnv::basic_stream : boost::noncopyable
{
    // C01. In string-to-type conversions we must ensure that the "string" is a CONTIGUOUS ARRAY of
    //      characters because "ibuffer_type" uses/relies on that (it deals with char_type*).
    //      Consequently, trying to generalize the supplied string as a range is misleading and, in fact,
    //      wrong, as ranges are not required to be contiguous as strings.
    //      Therefore, for string-to-type conversions we ON PURPOSE only take "char_type const*"
    //      (and, of course, std::string) to highlight that fact and NOT to mislead the user into believing that
    //      a non-contiguous range (like std::list<char_type>) can be passed as well.
    // C02. On the other hand, the type-to-string conversions ARE GENERALIZED on the string type.
    //      That allows us to use other than std::string types for output .
    // C11. Use the provided "string_in" as the input (read-from) buffer and, consequently,
    //      avoid the overhead associated with stream_.str(string_in) --
    //      copying of the content into internal buffer.
    // C12. The "strbuf.gptr() != strbuf.egptr()" check replaces "istream.eof() != true"
    //      which for some reason does not work when we try converting the "true" string
    //      to "bool" with std::boolalpha set. Seems that istream state gets unsynced compared
    //      to the actual underlying buffer.

    typedef Char                                 char_type;
    typedef boost::cnv::basic_stream<char_type>  this_type;
    typedef std::basic_stringstream<char_type> stream_type;
    typedef std::basic_istream<char_type>     istream_type;
    typedef std::basic_streambuf<char_type>    buffer_type;
    typedef std::basic_string<char_type>       string_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    struct ibuffer_type : public buffer_type
    {
        using buffer_type::eback;
        using buffer_type::gptr;
        using buffer_type::egptr;

        ibuffer_type(char_type const* beg) // Contiguous(!) range.
        {
            std::size_t sz = trait::size(beg);
            char_type* b = const_cast<char_type*>(beg);
            char_type* e = b + sz;

            buffer_type::setg(b, b, e);
        }
    };
    struct obuffer_type : public buffer_type
    {
        using buffer_type::pbase;
        using buffer_type::pptr;
        using buffer_type::epptr;
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

    template<typename TypeIn, typename StringOut>
    void
    operator()(TypeIn const& value_in, boost::optional<StringOut/*C02*/>& string_out) const
    {
        stream_.clear();            // Clear the flags
        stream_.str(string_type()); // Clear/empty the content of the stream

        if (!(stream_ << value_in).fail())
        {
            buffer_type*     buf = stream_.rdbuf();
            obuffer_type*   obuf = static_cast<obuffer_type*>(buf);
            char_type const* beg = obuf->pbase();
            char_type const* end = obuf->pptr();

            string_out = StringOut(beg, end); // Instead of stream_.str();
        }
    }

    template<typename TypeOut>
    void
    operator()(string_type const& string_in/*C01*/, boost::optional<TypeOut>& result_out) const
    {
        this->operator()(string_in.c_str(), result_out);
    }

    template<typename TypeOut>
    void
    operator()(char_type const* string_in/*C01*/, boost::optional<TypeOut>& result_out) const
    {
        istream_type& istream = stream_;
        buffer_type*   oldbuf = istream.rdbuf();
        ibuffer_type   newbuf (string_in); //C11

        istream.rdbuf(&newbuf);
        istream.clear(); // Clear the flags

        istream >> *(result_out = boost::make_default<TypeOut>());

        if (istream.fail() || newbuf.gptr() != newbuf.egptr()/*C12*/)
            result_out = boost::none;

        istream.rdbuf(oldbuf);
    }

    this_type& operator() (std::locale const& locale) { return (stream_.imbue(locale), *this); }
    this_type& operator() (manipulator_type m) { return (m(stream_), *this); }

    template<typename Manipulator>
    this_type& operator()(Manipulator m) { return (stream_ >> m, *this); }

    CONVERTER_PARAM_FUNC(locale, std::locale const) { return (stream_.imbue(arg[cnv::parameter::locale]), *this); }
    CONVERTER_PARAM_FUNC(precision, int const)      { return (stream_.precision(arg[cnv::parameter::precision]), *this); }
    CONVERTER_PARAM_FUNC(precision,       int)      { return (stream_.precision(arg[cnv::parameter::precision]), *this); }
    CONVERTER_PARAM_FUNC(width, int const)          { return (stream_.width(arg[cnv::parameter::width]), *this); }
    CONVERTER_PARAM_FUNC(fill, char const)          { return (stream_.fill(arg[cnv::parameter::fill]), *this); }
    CONVERTER_PARAM_FUNC(uppercase, bool const)
    {
        bool uppercase = arg[cnv::parameter::uppercase];
        uppercase ? (void) stream_.setf(std::ios::uppercase) : stream_.unsetf(std::ios::uppercase);
        return *this;
    }
    CONVERTER_PARAM_FUNC(skipws, bool const)
    {
        bool skipws = arg[cnv::parameter::skipws];
        skipws ? (void) stream_.setf(std::ios::skipws) : stream_.unsetf(std::ios::skipws);
        return *this;
    }
    CONVERTER_PARAM_FUNC(adjust, boost::cnv::adjust::type const)
    {
        cnv::adjust::type adjust = arg[cnv::parameter::adjust];

        /**/ if (adjust == cnv::adjust:: left) stream_.setf(std::ios::adjustfield, std::ios:: left);
        else if (adjust == cnv::adjust::right) stream_.setf(std::ios::adjustfield, std::ios::right);
        else BOOST_ASSERT(!"Not implemented");

        return *this;
    }
    CONVERTER_PARAM_FUNC(base, boost::cnv::base::type const)
    {
        cnv::base::type base = arg[cnv::parameter::base];
        
        /**/ if (base == cnv::base::dec) std::dec(stream_);
        else if (base == cnv::base::hex) std::hex(stream_);
        else if (base == cnv::base::oct) std::oct(stream_);
        else BOOST_ASSERT(!"Not implemented");
        
        return *this;
    }
    CONVERTER_PARAM_FUNC(notation, boost::cnv::notation::type const)
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
