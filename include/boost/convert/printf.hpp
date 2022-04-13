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

#define BOOST_CNV_PARAM_SET(param_name)   \
    template <typename argument_pack>     \
    void set_(                            \
        argument_pack const& arg,         \
        cnv::parameter::type::param_name, \
        mpl::true_)

#define BOOST_CNV_PARAM_TRY(param_name)     \
    this->set_(                             \
        arg,                                \
        cnv::parameter::type::param_name(), \
        typename mpl::has_key<              \
            argument_pack, cnv::parameter::type::param_name>::type());

struct boost::cnv::printf : boost::cnv::cnvbase<boost::cnv::printf>
{
    using this_type = boost::cnv::printf;
    using base_type = boost::cnv::cnvbase<this_type>;

    using base_type::operator();

    template<typename argument_pack>
    typename std::enable_if_t<boost::parameter::is_argument_pack<argument_pack>::value, this_type&>
    operator()(argument_pack const& arg)
    {
        base_type::operator()(arg);
        BOOST_CNV_PARAM_TRY(notation);

        return *this;
    }

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
    char const* printf_format(int pos) const
    {
        char const* d_fmt[] = { printf_format_float_, printf_format_float_, "%.*d", "%.*u", "%.*hd", "%.*hu", "%.*ld", "%.*lu" }; // Must match managed_types
        char const* x_fmt[] = { printf_format_float_, printf_format_float_, "%.*x", "%.*x", "%.*hx", "%.*hx", "%.*lx", "%.*lx" }; // Must match managed_types
        char const* o_fmt[] = { printf_format_float_, printf_format_float_, "%.*o", "%.*o", "%.*ho", "%.*ho", "%.*lo", "%.*lo" }; // Must match managed_types
        char const*     fmt = base_ == boost::cnv::base::dec ? d_fmt[pos]
                            : base_ == boost::cnv::base::hex ? x_fmt[pos]
                            : base_ == boost::cnv::base::oct ? o_fmt[pos]
                            : (BOOST_ASSERT(0), nullptr);
        return fmt;
    }
    char const* sscanf_format(int pos) const
    {
        char const* d_fmt[] = { sscanf_format_double_, sscanf_format_float_, "%d", "%u", "%hd", "%hu", "%ld", "%lu" }; // Must match managed_types
        char const* x_fmt[] = { sscanf_format_double_, sscanf_format_float_, "%x", "%x", "%hx", "%hx", "%lx", "%lx" }; // Must match managed_types
        char const* o_fmt[] = { sscanf_format_double_, sscanf_format_float_, "%o", "%o", "%ho", "%ho", "%lo", "%lo" }; // Must match managed_types
        char const*     fmt = base_ == boost::cnv::base::dec ? d_fmt[pos]
                            : base_ == boost::cnv::base::hex ? x_fmt[pos]
                            : base_ == boost::cnv::base::oct ? o_fmt[pos]
                            : (BOOST_ASSERT(0), nullptr);
        return fmt;
    }
    
    using base_type::set_;
    
    BOOST_CNV_PARAM_SET (notation)
    {
        cnv::notation notation = arg[cnv::parameter::notation];

        if (notation == cnv::notation::fixed)
        {
            printf_format_float_ = "%.*f";
            sscanf_format_float_ = "%f";
            sscanf_format_double_ = "%lf";
        }
        else if (notation == cnv::notation::scientific)
        {
            printf_format_float_ = "%.*e";
            sscanf_format_float_ = "%e";
            sscanf_format_double_ = "%le";
        }
        else if (notation == cnv::notation::hex)
        {
            printf_format_float_ = "%.*a";
            sscanf_format_float_ = "%a";
            sscanf_format_double_ = "%la";
        }
        else BOOST_ASSERT(!"Not implemented");
    }

    char const* printf_format_float_     = "%.*f";
    char const* sscanf_format_float_     = "%f";
    char const* sscanf_format_double_    = "%lf";
};

#undef BOOST_CNV_PARAM_SET
#undef BOOST_CNV_PARAM_TRY

#endif // BOOST_CONVERT_PRINTF_HPP
