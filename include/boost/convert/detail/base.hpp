// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CONVERTER_BASE_HPP
#define BOOST_CONVERT_CONVERTER_BASE_HPP

#include <boost/convert/parameters.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/as_literal.hpp>
#include <cctype>
#include <cstring>

namespace boost { namespace cnv { namespace detail
{
    typedef std::pair<char*, char*>  str_range;

    using boost::parameter::aux::tag;
    namespace ARG = boost::cnv::parameter;

    template<typename> struct cnvbase;

    template<typename string_type>
    struct is_string
    {
        static bool const value = true; // TBD
    };
}}}

#define BOOST_CNV_STRING_OP                                         \
    template<typename string_type>                                  \
    typename boost::enable_if<is_string<string_type>, void>::type   \
    operator()

#define BOOST_CNV_PARAM_OP(param_name, param_type) \
    derived_type& operator()(tag<ARG::type::param_name, param_type>::type const& arg)

template<typename derived_type>
struct boost::cnv::detail::cnvbase
{
    typedef cnvbase                  this_type;
    typedef int                       int_type;
    typedef unsigned int             uint_type;
    typedef long int                 lint_type;
    typedef unsigned long int       ulint_type;
    typedef short int                sint_type;
    typedef unsigned short int      usint_type;
    typedef long long int           llint_type;
    typedef unsigned long long int ullint_type;
    typedef float                     flt_type;
    typedef double                    dbl_type;
    typedef long double              ldbl_type;

    BOOST_CNV_STRING_OP ( int_type v, optional<string_type>& r) const { to_str_(v, r); }
    BOOST_CNV_STRING_OP (lint_type v, optional<string_type>& r) const { to_str_(v, r); }
    BOOST_CNV_STRING_OP ( dbl_type v, optional<string_type>& r) const { to_str_(v, r); }
    BOOST_CNV_STRING_OP (ldbl_type v, optional<string_type>& r) const { to_str_(v, r); }

    BOOST_CNV_STRING_OP (string_type const& s, optional< int_type>& r) const { dncast().str_to(s, r); }
    BOOST_CNV_STRING_OP (string_type const& s, optional<lint_type>& r) const { dncast().str_to(s, r); }
    BOOST_CNV_STRING_OP (string_type const& s, optional< dbl_type>& r) const { dncast().str_to(s, r); }
    BOOST_CNV_STRING_OP (string_type const& s, optional<ldbl_type>& r) const { dncast().str_to(s, r); }

    BOOST_CNV_PARAM_OP (locale,  std::locale const) { locale_    = arg[ARG::   locale]; return dncast(); }
    BOOST_CNV_PARAM_OP (base,     base::type const) { base_      = arg[ARG::     base]; return dncast(); }
    BOOST_CNV_PARAM_OP (adjust, adjust::type const) { adjust_    = arg[ARG::   adjust]; return dncast(); }
    BOOST_CNV_PARAM_OP (precision,       int const) { precision_ = arg[ARG::precision]; return dncast(); }
    BOOST_CNV_PARAM_OP (precision,             int) { precision_ = arg[ARG::precision]; return dncast(); }
    BOOST_CNV_PARAM_OP (uppercase,      bool const) { uppercase_ = arg[ARG::uppercase]; return dncast(); }
    BOOST_CNV_PARAM_OP (skipws,         bool const) { skipws_    = arg[ARG::   skipws]; return dncast(); }
    BOOST_CNV_PARAM_OP (width,           int const) { width_     = arg[ARG::    width]; return dncast(); }
    BOOST_CNV_PARAM_OP (fill,           char const) {  fill_     = arg[ARG::     fill]; return dncast(); }

    protected:

    cnvbase()
    :
        base_       (10),
        skipws_     (false),
        precision_  (0),
        uppercase_  (false),
        width_      (0),
        fill_       (' '),
        adjust_     (boost::cnv::adjust::right)
    {}

    template<typename type_in, typename string_type>
    void
    to_str_(type_in value_in, optional<string_type>& result_out) const
    {
        char buf[bufsize_];
        str_range    range = dncast().to_str(value_in, buf);
        char*          beg = range.first;
        char*          end = range.second;

        if (beg < end)
        {
            format_(buf, beg, end);

            result_out = string_type(beg, end);
        }
    }

    void
    format_(char* buf, char*& beg, char*& end) const
    {
        if (uppercase_)
        {
            for (char* p = beg; p < end; ++p) *p = toupper(*p);
        }
        if (width_)
        {
            int const num_fillers = std::max(0, width_ - (end - beg));
            int const    num_left = adjust_ == boost::cnv::adjust::left ? 0
                                  : adjust_ == boost::cnv::adjust::right ? num_fillers
                                  : (num_fillers / 2);
            int const   num_right = num_fillers - num_left;
            int const    str_size = end - beg;
            bool const       move = (beg < buf + num_left) // No room for left fillers
                                 || (buf + bufsize_ < end + num_right); // No room for right fillers
            if (move)
            {
                std::memmove(buf + num_left, beg, str_size);
                beg = buf + num_left;
                end = beg + str_size;
            }
            for (int k = 0; k <  num_left; *(--beg) = fill_, ++k);
            for (int k = 0; k < num_right; *(end++) = fill_, ++k);
        }
    }

    derived_type const& dncast () const { return *static_cast<derived_type const*>(this); }
    derived_type&       dncast ()       { return *static_cast<derived_type*>(this); }

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20(10) or 32(2) characters)
    // double (8 bytes) max is 316 chars
    static int const bufsize_ = 1024;
    int                 base_;
    bool              skipws_;
    int            precision_;
    bool           uppercase_;
    int                width_;
    int                 fill_;
    adjust::type      adjust_;
    std::locale       locale_;
};

#undef BOOST_CNV_STRING_OP
#undef BOOST_CNV_PARAM_OP

#define CONVERTER_PARAM_FUNC(PARAM_NAME, PARAM_TYPE)    \
    this_type&                                          \
    operator()(boost::parameter::aux::tag<boost::cnv::parameter::type::PARAM_NAME, PARAM_TYPE>::type const& arg)

#endif // BOOST_CONVERT_CONVERTER_BASE_HPP
