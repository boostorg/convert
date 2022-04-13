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
    char_cptr fmt(
        int pos,
        fmt_type fxd_d_fmt, fmt_type fxd_x_fmt, fmt_type fxd_o_fmt,
        fmt_type sci_d_fmt, fmt_type sci_x_fmt, fmt_type sci_o_fmt,
        fmt_type hex_d_fmt, fmt_type hex_x_fmt, fmt_type hex_o_fmt) const
    {
        if (notation_ == notation::fixed)
            return base_ == base::dec ? fxd_d_fmt[pos]
                 : base_ == base::hex ? fxd_x_fmt[pos]
                 : base_ == base::oct ? fxd_o_fmt[pos]
                 : (BOOST_ASSERT(0), nullptr);

        if (notation_ == notation::scientific)
            return base_ == base::dec ? sci_d_fmt[pos]
                 : base_ == base::hex ? sci_x_fmt[pos]
                 : base_ == base::oct ? sci_o_fmt[pos]
                 : (BOOST_ASSERT(0), nullptr);

        if (notation_ == notation::hex)
            return base_ == base::dec ? hex_d_fmt[pos]
                 : base_ == base::hex ? hex_x_fmt[pos]
                 : base_ == base::oct ? hex_o_fmt[pos]
                 : (BOOST_ASSERT(0), nullptr);

        return (BOOST_ASSERT(0), nullptr);
    }
    char_cptr printf_format(int pos) const
    {
        char_cptr constexpr fxd_d_fmt[] = { "%.*f", "%.*f", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }; // Must match managed_types
        char_cptr constexpr fxd_x_fmt[] = { "%.*f", "%.*f", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }; // Must match managed_types
        char_cptr constexpr fxd_o_fmt[] = { "%.*f", "%.*f", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }; // Must match managed_types
        char_cptr constexpr sci_d_fmt[] = { "%.*e", "%.*e", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }; // Must match managed_types
        char_cptr constexpr sci_x_fmt[] = { "%.*e", "%.*e", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }; // Must match managed_types
        char_cptr constexpr sci_o_fmt[] = { "%.*e", "%.*e", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }; // Must match managed_types
        char_cptr constexpr hex_d_fmt[] = { "%.*a", "%.*a", "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }; // Must match managed_types
        char_cptr constexpr hex_x_fmt[] = { "%.*a", "%.*a", "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }; // Must match managed_types
        char_cptr constexpr hex_o_fmt[] = { "%.*a", "%.*a", "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }; // Must match managed_types

        return fmt(pos,
                   fxd_d_fmt, fxd_x_fmt, fxd_o_fmt,
                   sci_d_fmt, sci_x_fmt, sci_o_fmt,
                   hex_d_fmt, hex_x_fmt, hex_o_fmt);
    }
    char_cptr sscanf_format(int pos) const
    {
        char_cptr constexpr fxd_d_fmt[] = { "%lf", "%f", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }; // Must match managed_types
        char_cptr constexpr fxd_x_fmt[] = { "%lf", "%f", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }; // Must match managed_types
        char_cptr constexpr fxd_o_fmt[] = { "%lf", "%f", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }; // Must match managed_types
        char_cptr constexpr sci_d_fmt[] = { "%le", "%e", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }; // Must match managed_types
        char_cptr constexpr sci_x_fmt[] = { "%le", "%e", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }; // Must match managed_types
        char_cptr constexpr sci_o_fmt[] = { "%le", "%e", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }; // Must match managed_types
        char_cptr constexpr hex_d_fmt[] = { "%la", "%a", "%d", "%u", "%hd", "%hu", "%ld", "%lu" }; // Must match managed_types
        char_cptr constexpr hex_x_fmt[] = { "%la", "%a", "%x", "%x", "%hx", "%hx", "%lx", "%lx" }; // Must match managed_types
        char_cptr constexpr hex_o_fmt[] = { "%la", "%a", "%o", "%o", "%ho", "%ho", "%lo", "%lo" }; // Must match managed_types

        return fmt(pos,
                   fxd_d_fmt, fxd_x_fmt, fxd_o_fmt,
                   sci_d_fmt, sci_x_fmt, sci_o_fmt,
                   hex_d_fmt, hex_x_fmt, hex_o_fmt);
    }
};

#endif // BOOST_CONVERT_PRINTF_HPP
