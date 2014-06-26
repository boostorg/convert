// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <limits>
#include <stdlib.h>
#include <limits.h>

namespace boost { namespace cnv
{
    struct strtol;
}}

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase<boost::cnv::strtol>
{
    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

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

    void operator()(     int v, boost::optional<std::string>& res) const { res = to_str(v); }
    void operator()(long int v, boost::optional<std::string>& res) const { res = to_str(v); }

    void operator()(double value_in, boost::optional<std::string>& result_out) const
    {
        result_out = this_type::dtostr(value_in, precision_);
    }
    template<typename T> std::string to_str (T) const;
    /******************/ std::string dtostr (double, unsigned int precision) const;

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20 characters)
    // double (8 bytes) max is 316 chars
    static int const bufsize = 1024;
};

template<typename Type>
inline
std::string
boost::cnv::strtol::to_str(Type value) const
{
    char   buf[bufsize];
    int const is_negative = (value < 0) ? (value = -value, 1) : 0;
    char* const buf_beg = buf + is_negative;
    char*           end = buf + bufsize / 2;
    char*           beg = end;
    
    for (; value && buf_beg < beg; value /= base_)
    { 
        int remainder = value % base_;
        
        if (remainder < 10) *(--beg) = remainder += '0';
        else                *(--beg) = remainder += 'A' - 10;
    } 
    if (is_negative)
        *(--beg) = '-';

    if (width_)
    {
        int const num_fillers = width_ - (end - beg);
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
    int const is_negative = (value < 0) ? (value = -value, 1) : 0;
    int         int_value = value;
    double       fraction = value - int_value;
    int const        base = 10;
    char     buf[bufsize];
    char* const   buf_beg = buf + is_negative;
    char*             end = buf + bufsize / 2;
    char*             beg = end;

    for (; int_value /*&& buf_beg < beg*/; int_value /= base)
    {
        int remainder = int_value % base;

        if (remainder < 10) *(--beg) = remainder += '0';
        else                *(--beg) = remainder += 'A' - 10;
    }
    if (precision) *(end++) = '.';

    for (; fraction && precision; --precision)
    {
        int digit = fraction *= base;
        fraction -= digit;

        if (digit < 10) *(end++) = digit += '0';
        else            *(end++) = digit += 'A' - 10;
    }
    // Rounding up: from the middle (including) adds to the next bigger number on the axis.
    // For positive: 5,6,7,8,9 add to the next on the axis.
    // For negative: 5,4,3,2,1 add to the next on the axis, i.e. absolute numeric value is smaller.
    if (fraction)
        if (is_negative ? (0.5 < fraction) : (0.5 <= fraction)) // Extra digit is not 0. Round half-up.
    {
        char* prev = end;

        while (beg <= (*--prev == '.' ? --prev : prev))
        {
            if (*prev == '9') *prev = '0';
            else
            {
                ++*prev;
                break;
            }
        }
        if (prev < beg)
            *(beg = prev) = '1';
    }
    for (; precision; --precision)
        *(end++) = '0';

    if (*beg == '.') *(--beg) = '0';
    if (is_negative) *(--beg) = '-';

    if (width_)
    {
        int const num_fillers = width_ - (end - beg);
        bool const      right = adjustment_ == boost::cnv::adjustment::right;

        if (right) for (int k = 0; k < num_fillers; *(--beg) = fill_, ++k);
        else       for (int k = 0; k < num_fillers; *(end++) = fill_, ++k);
    }
    return std::string(beg, end);
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
