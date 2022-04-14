// Copyright (c) 2009-2020 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_PRINTF_HPP
#define BOOST_CONVERT_PRINTF_HPP

#include <boost/convert/base.hpp>
#include <boost/make_default.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <string>
#include <cstdio>

namespace boost { namespace cnv
{
    struct printf;
}}

struct boost::cnv::printf : boost::cnv::cnvbase<boost::cnv::printf>
{
    using this_type = boost::cnv::printf;
    using base_type = boost::cnv::cnvbase<this_type>;
    using  fmt_type = char_cptr const*;

    using base_type::operator();

    template<typename in_type>
    cnv::range<char*>
    to_str(in_type value_in, char* buf) const
    {
        char const* fmt = printf_format(pos<in_type>());
        int   num_chars = snprintf(buf, bufsize_, fmt, precision_, value_in);
        bool    success = num_chars < bufsize_;

        return cnv::range<char*>(buf, success ? (buf + num_chars) : buf);
    }
    template<typename string_type, typename out_type>
    void
    str_to(cnv::range<string_type> range, optional<out_type>& result_out) const
    {
        out_type result = boost::make_default<out_type>();
        char const* fmt = sscanf_format(pos<out_type>());
        int    num_read = sscanf(&*range.begin(), fmt, &result);

        if (num_read == 1)
            result_out = result;
    }

    private:

    template<typename Type> int pos() const
    {
        using managed_types = boost::mpl::vector<double, float, int,
                                  unsigned int, short int, unsigned short int,
                                  long int, unsigned long int>;
        using type_iterator = typename boost::mpl::find<managed_types, Type>::type;
        using      type_pos = typename type_iterator::pos;

        return type_pos::value;
    }
    char_cptr printf_format(int pos) const
    {
        char_cptr constexpr d_fmt[3][8] =
        {
    /*fxd*/ { "%.*f", "%.*f", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }, // Must match managed_types
    /*sci*/ { "%.*e", "%.*e", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }, // Must match managed_types
    /*hex*/ { "%.*a", "%.*a", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }  // Must match managed_types
        };
        char_cptr constexpr x_fmt[3][8] =
        {
            { "%.*f", "%.*f", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }, // Must match managed_types
            { "%.*e", "%.*e", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }, // Must match managed_types
            { "%.*a", "%.*a", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }  // Must match managed_types
        };
        char_cptr constexpr o_fmt[3][8] =
        {
            { "%.*f", "%.*f", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }, // Must match managed_types
            { "%.*e", "%.*e", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }, // Must match managed_types
            { "%.*a", "%.*a", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }  // Must match managed_types
        };
        return base_ == base::dec ? d_fmt[int(notation_)][pos]
             : base_ == base::hex ? x_fmt[int(notation_)][pos]
             : base_ == base::oct ? o_fmt[int(notation_)][pos]
             : (BOOST_ASSERT(0), nullptr);
    }
    char_cptr sscanf_format(int pos) const
    {
        char_cptr constexpr d_fmt[3][8] =
        {
            { "%lf", "%f", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }, // Must match managed_types
            { "%le", "%e", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }, // Must match managed_types
            { "%la", "%a", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }  // Must match managed_types
        };
        char_cptr constexpr x_fmt[3][8] =
        {
            { "%lf", "%f", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }, // Must match managed_types
            { "%le", "%e", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }, // Must match managed_types
            { "%la", "%a", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }  // Must match managed_types
        };
        char_cptr constexpr o_fmt[3][8] =
        {
            { "%lf", "%f", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }, // Must match managed_types
            { "%le", "%e", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }, // Must match managed_types
            { "%la", "%a", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }  // Must match managed_types
        };
        return base_ == base::dec ? d_fmt[int(notation_)][pos]
             : base_ == base::hex ? x_fmt[int(notation_)][pos]
             : base_ == base::oct ? o_fmt[int(notation_)][pos]
             : (BOOST_ASSERT(0), nullptr);
    }
};

#endif // BOOST_CONVERT_PRINTF_HPP
