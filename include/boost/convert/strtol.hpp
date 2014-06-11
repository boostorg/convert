// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <stdlib.h>

namespace boost { namespace cnv
{
    struct strtol;
}}

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase
{
    typedef boost::cnv::strtol          this_type;
    typedef boost::cnv::detail::cnvbase base_type;

    CONVERT_FUNC_SET_BASE;
    CONVERT_FUNC_SET_PRECISION;
    CONVERT_FUNC_SET_UPPERCASE;

    void operator()(std::string const& v, boost::optional<int>&               r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<long int>&          r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<unsigned long int>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<double>&            r) const { operator()(v.c_str(), r); }

    void operator()(char const* value_in, boost::optional<double>& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        double const result = ::strtod(value_in, &cnv_end);

        if (cnv_end == str_end)
            result_out = result;
    }
    void operator()(char const* value_in, boost::optional<int>& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        long int     result = ::strtol(value_in, &cnv_end, base_);

        if (INT_MIN <= result && result <= INT_MAX && cnv_end == str_end)
            result_out = int(result);
    }
    void operator()(char const* value_in, boost::optional<long int>& result_out) const
    {
        char const*   str_end = value_in + strlen(value_in);
        char*         cnv_end = 0;
        long int const result = ::strtol(value_in, &cnv_end, base_);

        if (result != LONG_MIN && result != LONG_MAX && cnv_end == str_end)
            result_out = result;
    }
    void operator()(char const* value_in, boost::optional<unsigned long int>& result_out) const
    {
        char const*            str_end = value_in + strlen(value_in);
        char*                  cnv_end = 0;
        unsigned long int const result = ::strtoul(value_in, &cnv_end, base_);

        if (result != ULONG_MAX && cnv_end == str_end)
            result_out = result;
    }
    void operator()(int value_in, boost::optional<std::string>& result_out) const
    {
        result_out = this_type::ltostr(value_in, base_);
    }
    static std::string ltostr(long int num, unsigned int base);
};

inline
std::string
boost::cnv::strtol::ltostr(long int num, unsigned int base) 
{ 
    if (!num) return "0";

    int const     strsz = 256;
    char     str[strsz];
    int const sign_size = (num < 0) ? (num = -num, 1) : 0;
    char* const     beg = str + sign_size;
    char* const     end = str + strsz;
    char*           pos = end;
    
    for (; num && beg < pos; num /= base) 
    { 
        int remainder = num % base;
        
        if (remainder < 10) *(--pos) = remainder + '0'; 
        else                *(--pos) = remainder - 10 + 'A'; 
    } 
    if (sign_size) 
        *(--pos) = '-'; 
    
    return std::string(pos, end); 
} 

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
