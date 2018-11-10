// Copyright (c) 2009-2016 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
#define BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP

#include <boost/convert/parameters.hpp>
#include <boost/convert/detail/is_string.hpp>
#include <boost/make_default.hpp>
#include <boost/noncopyable.hpp>
#include <sstream>
#include <iomanip>

#define BOOST_CNV_STRING_ENABLE                                         \
    template<typename string_type, typename type>                       \
    typename boost::enable_if<cnv::is_string<string_type>, void>::type  \
    operator()

namespace boost { namespace cnv
{
    template <class Char> struct basic_stream;

    typedef boost::cnv::basic_stream<char>    cstream;
    typedef boost::cnv::basic_stream<wchar_t> wstream;
}}

#include <boost/parameter/is_argument_pack.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_key.hpp>

template <class Char>
struct boost::cnv::basic_stream : boost::noncopyable
{
    // C01. In string-to-type conversions the "string" must be a CONTIGUOUS ARRAY of
    //      characters because "ibuffer_type" uses/relies on that (it deals with char_type*).
    // C02. Use the provided "string_in" as the input (read-from) buffer and, consequently,
    //      avoid the overhead associated with stream_.str(string_in) --
    //      copying of the content into internal buffer.
    // C03. The "strbuf.gptr() != strbuf.egptr()" check replaces "istream.eof() != true"
    //      which for some reason does not work when we try converting the "true" string
    //      to "bool" with std::boolalpha set. Seems that istream state gets unsynced compared
    //      to the actual underlying buffer.

    typedef Char                                 char_type;
    typedef boost::cnv::basic_stream<char_type>  this_type;
    typedef std::basic_stringstream<char_type> stream_type;
    typedef std::basic_istream<char_type>     istream_type;
    typedef std::basic_streambuf<char_type>    buffer_type;
    typedef std::basic_string<char_type>       stdstr_type;
    typedef std::ios_base& (*manipulator_type)(std::ios_base&);

    struct ibuffer_type : public buffer_type
    {
        using buffer_type::eback;
        using buffer_type::gptr;
        using buffer_type::egptr;

        ibuffer_type(char_type const* beg, std::size_t sz) //C01
        {
            char_type* b = const_cast<char_type*>(beg);

            buffer_type::setg(b, b, b + sz);
        }
    };
    struct obuffer_type : public buffer_type
    {
        using buffer_type::pbase;
        using buffer_type::pptr;
        using buffer_type::epptr;
    };

    basic_stream() : stream_(std::ios_base::in | std::ios_base::out) {}
#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )
    basic_stream(this_type&& other) : stream_(std::move(other.stream_)) {}
#endif

    BOOST_CNV_STRING_ENABLE(type const& v, optional<string_type>& s) const { to_str(v, s); }
    BOOST_CNV_STRING_ENABLE(string_type const& s, optional<type>& r) const { str_to(cnv::range<string_type const>(s), r); }
    // Resolve ambiguity of string-to-string
    template<typename type> void operator()(  char_type const* s, optional<type>& r) const { str_to(cnv::range< char_type const*>(s), r); }
    template<typename type> void operator()(stdstr_type const& s, optional<type>& r) const { str_to(cnv::range<stdstr_type const>(s), r); }

    // Formatters
    template <typename manipulator>
    typename ::boost::disable_if<
        ::boost::parameter::is_argument_pack<manipulator>
      , this_type&
    >::type
    operator()(manipulator m)
    {
        this->stream_ << m;
        return *this;
    }

    this_type& operator() (manipulator_type m) { return (m(stream_), *this); }
    this_type& operator() (std::locale const& l) { return (stream_.imbue(l), *this); }

    private:

    template <typename argument_pack, typename keyword_tag>
    void _assign(argument_pack const& arg, keyword_tag, ::boost::mpl::false_)
    {
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::locale
      , ::boost::mpl::true_
    )
    {
        this->stream_.imbue(arg[::boost::cnv::parameter::locale]);
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::precision
      , ::boost::mpl::true_
    )
    {
        this->stream_.precision(arg[::boost::cnv::parameter::precision]);
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::width
      , ::boost::mpl::true_
    )
    {
        this->stream_.width(arg[::boost::cnv::parameter::width]);
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::fill
      , ::boost::mpl::true_
    )
    {
        this->stream_.fill(arg[::boost::cnv::parameter::fill]);
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::uppercase
      , ::boost::mpl::true_
    )
    {
        if (arg[::boost::cnv::parameter::uppercase])
        {
            this->stream_.setf(::std::ios::uppercase);
        }
        else
        {
            this->stream_.unsetf(::std::ios::uppercase);
        }
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::skipws
      , ::boost::mpl::true_
    )
    {
        if (arg[::boost::cnv::parameter::skipws])
        {
            this->stream_.setf(::std::ios::skipws);
        }
        else
        {
            this->stream_.unsetf(::std::ios::skipws);
        }
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::adjust
      , ::boost::mpl::true_
    )
    {
        ::boost::cnv::adjust::type adjust = arg[
            ::boost::cnv::parameter::adjust
        ];

        /**/ if (adjust == ::boost::cnv::adjust::left)
        {
            this->stream_.setf(::std::ios::adjustfield, ::std::ios::left);
        }
        else if (adjust == ::boost::cnv::adjust::right)
        {
            this->stream_.setf(::std::ios::adjustfield, ::std::ios::right);
        }
        else BOOST_ASSERT(!"Not implemented");
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::base
      , ::boost::mpl::true_
    )
    {
        ::boost::cnv::base::type base = arg[::boost::cnv::parameter::base];

        /**/ if (base == ::boost::cnv::base::dec) ::std::dec(this->stream_);
        else if (base == ::boost::cnv::base::hex) ::std::hex(this->stream_);
        else if (base == ::boost::cnv::base::oct) ::std::oct(this->stream_);
        else BOOST_ASSERT(!"Not implemented");
    }

    template <typename argument_pack>
    void
    _assign(
        argument_pack const& arg
      , ::boost::cnv::parameter::type::notation
      , ::boost::mpl::true_
    )
    {
        ::boost::cnv::notation::type notation = arg[
            ::boost::cnv::parameter::notation
        ];

        /**/ if (notation == ::boost::cnv::notation::fixed)
        {
            ::std::fixed(this->stream_);
        }
        else if (notation == ::boost::cnv::notation::scientific)
        {
            ::std::scientific(this->stream_);
        }
        else BOOST_ASSERT(!"Not implemented");
    }

    public:

    template <typename argument_pack>
    typename ::boost::enable_if<
        ::boost::parameter::is_argument_pack<argument_pack>
      , this_type&
    >::type
    operator()(argument_pack const& arg)
    {
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::precision()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::precision
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::width()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::width
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::fill()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::fill
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::uppercase()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::uppercase
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::skipws()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::skipws
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::adjust()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::adjust
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::base()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::base
            >::type()
        );
        this->_assign(
            arg
          , ::boost::cnv::parameter::type::notation()
          , typename ::boost::mpl::has_key<
                argument_pack
              , ::boost::cnv::parameter::type::notation
            >::type()
        );
        return *this;
    }

    private:

    template<typename string_type, typename out_type> void str_to(cnv::range<string_type>, optional<out_type>&) const;
    template<typename string_type, typename  in_type> void to_str(in_type const&, optional<string_type>&) const;

    mutable stream_type stream_;
};

template<typename char_type>
template<typename string_type, typename in_type>
inline
void
boost::cnv::basic_stream<char_type>::to_str(
    in_type const& value_in,
    boost::optional<string_type>& string_out) const
{
    stream_.clear();            // Clear the flags
    stream_.str(stdstr_type()); // Clear/empty the content of the stream

    if (!(stream_ << value_in).fail())
    {
        buffer_type*     buf = stream_.rdbuf();
        obuffer_type*   obuf = static_cast<obuffer_type*>(buf);
        char_type const* beg = obuf->pbase();
        char_type const* end = obuf->pptr();

        string_out = string_type(beg, end); // Instead of stream_.str();
    }
}

template<typename char_type>
template<typename string_type, typename out_type>
inline
void
boost::cnv::basic_stream<char_type>::str_to(
    boost::cnv::range<string_type> string_in,
    boost::optional<out_type>& result_out) const
{
    if (string_in.empty ()) return;

    istream_type& istream = stream_;
    buffer_type*   oldbuf = istream.rdbuf();
    char_type const*  beg = &*string_in.begin();
    std::size_t        sz = string_in.end() - string_in.begin();
    ibuffer_type   newbuf (beg, sz); //C02

    istream.rdbuf(&newbuf);
    istream.clear(); // Clear the flags

    istream >> *(result_out = boost::make_default<out_type>());

    if (istream.fail() || newbuf.gptr() != newbuf.egptr()/*C03*/)
        result_out = boost::none;

    istream.rdbuf(oldbuf);
}

#undef BOOST_CNV_STRING_ENABLE

#endif // BOOST_CONVERT_STRINGSTREAM_BASED_CONVERTER_HPP
