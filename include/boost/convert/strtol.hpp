// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <limits>

namespace boost { namespace cnv
{
    struct strtol;
}}

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase<boost::cnv::strtol>
{
    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;
    typedef std::pair<char*, char*>            iterator_pair;

    using base_type::operator();

    void operator()(std::string const& v, boost::optional<int>&               r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<long int>&          r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<unsigned long int>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<double>&            r) const { operator()(v.c_str(), r); }

    template<typename Type> void strtod_(char const* value_in, boost::optional<Type>& result_out) const
    {
        char const*      str_end = value_in + strlen(value_in);
        char*            cnv_end = 0;
        long double const result = ::strtold(value_in, &cnv_end);
        bool const          good = result != -HUGE_VALL && result != HUGE_VALL && cnv_end == str_end;
        Type const           max = std::numeric_limits<Type>::max();

        if (good && -max <= result && result <= max)
            result_out = Type(result);
        else
            ::printf("trouble\n");
    }
    template<typename Type> void strtol_(char const* value_in, boost::optional<Type>& result_out) const
    {
        char const*        str_end = value_in + strlen(value_in);
        char*              cnv_end = 0;
        long long int const result = ::strtoll(value_in, &cnv_end, base_);
        bool const            good = result != LLONG_MIN && result != LLONG_MAX && cnv_end == str_end;
        Type const             min = std::numeric_limits<Type>::min();
        Type const             max = std::numeric_limits<Type>::max();

        if (good && min <= result && result <= max)
            result_out = Type(result);
    }
    template<typename Type> void strtoul_(char const* value_in, boost::optional<Type>& result_out) const
    {
        char const*                 str_end = value_in + strlen(value_in);
        char*                       cnv_end = 0;
        unsigned long long int const result = ::strtoull(value_in, &cnv_end, base_);
        bool const                     good = result != ULLONG_MAX && cnv_end == str_end;
        Type const                      max = std::numeric_limits<Type>::max();

        if (good && result <= max)
            result_out = Type(result);
    }

    void operator()(char const* v, boost::optional<             float>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<            double>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<       long double>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<         short int>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<               int>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<          long int>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<unsigned short int>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, boost::optional<unsigned       int>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, boost::optional<unsigned  long int>& res) const { strtoul_ (v, res); }

    void operator()(     int v, boost::optional<std::string>& res) const { res = to_str(v, base_, width_); }
    void operator()(long int v, boost::optional<std::string>& res) const { res = to_str(v, base_, width_); }

    void operator()(double value_in, boost::optional<std::string>& result_out) const
    {
        result_out = this_type::dtostr(value_in, precision_);
    }
    template<typename T> iterator_pair to_str (T, int base, char*, int const) const;
    template<typename T> std::string   to_str (T, int base, int width) const;
    /******************/ std::string   dtostr (double, unsigned int precision) const;
};

template<typename Type>
inline
boost::cnv::strtol::iterator_pair
boost::cnv::strtol::to_str(Type value, int base, char* buf, int const buf_size) const
{ 
    int const sign_size = (value < 0) ? (value = -value, 1) : 0;
    char* const buf_beg = buf + sign_size;
    char*           end = buf + buf_size / 2;
    char*           beg = end;
    
    for (; value /*&& buf_beg < beg*/; value /= base)
    { 
        int remainder = value % base;
        
        if (remainder < 10) *(--beg) = remainder += '0';
        else                *(--beg) = remainder += 'A' - 10;
    } 
    if (sign_size) 
        *(--beg) = '-';

    return std::make_pair(beg, end);
}

template<typename Type>
inline
std::string
boost::cnv::strtol::to_str(Type value, int base, int width) const
{
    int const   strsz = 256;
    char   str[strsz];
    iterator_pair res = to_str(value, base_, str, strsz);
    char*         beg = res.first;
    char*         end = res.second;
    
    if (width)
    {
        int const num_fillers = width - (end - beg);
        bool const      right = adjustment_ == boost::cnv::adjustment::right;

        if (right) for (int k = 0; k < num_fillers; *(--beg) = fill_, ++k);
        else       for (int k = 0; k < num_fillers; *(end++) = fill_, ++k);
    }
    return std::string(beg, end);
} 

inline
std::string
boost::cnv::strtol::dtostr(double value,  unsigned int precision) const
{
    int const sign_size = (value < 0) ? (value = -value, 1) : 0;
    int       int_value = value;
    double     fraction = value - int_value;
    int const      base = 10;
    int const  buf_size = 256;
    char  buf[buf_size];
    char* const buf_beg = buf + sign_size;
    char*           end = buf + buf_size / 2;
    char*           beg = end;

    for (; int_value /*&& buf_beg < beg*/; int_value /= base)
    {
        int remainder = int_value % base;

        if (remainder < 10) *(--beg) = remainder += '0';
        else                *(--beg) = remainder += 'A' - 10;
    }
    *(end++) = '.';

    while ((fraction *= base) && precision--)
    {
        int digit = int(fraction) % base;
        fraction -= digit;

        if (digit < 10) *(end++) = digit += '0';
        else            *(end++) = digit += 'A' - 10;
    }
    if (fraction && 4 < int(fraction) % base) // Extra digit is =>5. Rounding up.
    {
        char* prev = end;

        while (beg <= (*--prev == '.' ? --prev : prev))
        {
            if (*prev != '9')
            {
                ++*prev;
                break;
            }
            else *prev = '0';
        }
        if (prev < beg)
            *(beg = prev) = '1';
    }
    if (*beg == '.') *(--beg) = '0';
    if (sign_size)   *(--beg) = '-';

    return std::string(beg, end);
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
