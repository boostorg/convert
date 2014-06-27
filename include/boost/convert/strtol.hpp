// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <limits>
#include <cmath>
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
    typedef int                                     int_type;
    typedef unsigned int                           uint_type;
    typedef long int                               lint_type;
    typedef unsigned long int                     ulint_type;
    typedef short int                              sint_type;
    typedef unsigned short int                    usint_type;

    using base_type::operator();

    void operator()(std::string const& v, boost::optional<  int_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional< sint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional< lint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional< uint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<usint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<ulint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<    double>& r) const { operator()(v.c_str(), r); }

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

    void operator()(char const* v, boost::optional<      float>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<     double>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<long double>& res) const { strtod_  (v, res); }
    void operator()(char const* v, boost::optional<   int_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<  sint_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<  lint_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, boost::optional<  uint_type>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, boost::optional< usint_type>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, boost::optional< ulint_type>& res) const { strtoul_ (v, res); }

    void operator()(     int v, boost::optional<std::string>& res) const { res = i_to_str(v); }
    void operator()(long int v, boost::optional<std::string>& res) const { res = i_to_str(v); }
    void operator()(  double v, boost::optional<std::string>& res) const { res = d_to_str(v); }

    template<typename T> std::string i_to_str (T) const;
    /******************/ std::string d_to_str (double) const;
    static bool                  round_up_abs (double const, unsigned int const, bool const);

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20 characters)
    // double (8 bytes) max is 316 chars
    static int const bufsize = 1024;
};

template<typename Type>
inline
std::string
boost::cnv::strtol::i_to_str(Type value) const
{
    char     buf[bufsize];
    int const is_negative = (value < 0) ? (value = -value, 1) : 0;
    char* const   buf_beg = buf + is_negative;
    char*             end = buf + bufsize / 2;
    char*             beg = end;

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
bool
boost::cnv::strtol::round_up_abs(double const fraction, unsigned int const precision, bool const is_negative)
{
    // C1. Bring forward the fraction coming right after precision digits.
    //     That is, say, fraction=0.234567, precision=2. Then remainder=0.4567
    // C2. ::round() returns the integral value that is nearest to x,
    //     with halfway cases rounded away from zero. Therefore,
    //          round( 0.4) =  0
    //          round( 0.5) =  1
    //          round( 0.6) =  1
    //          round(-0.4) =  0
    //          round(-0.5) = -1
    //          round(-0.6) = -1

    BOOST_ASSERT(precision < 13);

    long int const tens[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000 }; // max precision =12
    double  bring_forward = fraction * tens[precision];
    double      remainder = (bring_forward - std::floor(bring_forward)) * (is_negative ? -1 : 1); //C1.
//  bool     round_up_abs = int(::rint(remainder)) != 0;
    bool     round_up_abs = int(::round(remainder)) != 0; //C2

    return round_up_abs;
}

inline
std::string
boost::cnv::strtol::d_to_str(double value) const
{
    int const  is_negative = (value < 0) ? (value = -value, 1) : 0;
    double        int_part = std::floor(value);
    double        fraction = value - int_part;
    unsigned int precision = precision_;
    bool const    round_up = round_up_abs(fraction, precision, is_negative);
    int const         base = 10;
    char      buf[bufsize];
    char* const    buf_beg = buf + is_negative;
    char*              end = buf + bufsize / 2;
    char*              beg = end;

    for (; 1 <= int_part /*&& buf_beg < beg*/; int_part /= base)
    {
        int digit = int_part - std::floor(int_part / base) * base;

        if (digit < 10) *(--beg) = digit += '0';
        else            *(--beg) = digit += 'A' - 10;
    }
    if (beg == end)  *(--beg) = '0';
    if (is_negative) *(--beg) = '-';
    if (precision)   *(end++) = '.';

    for (; fraction && precision; --precision)
    {
        int digit = fraction *= base;
        fraction -= digit;

        if (digit < 10) *(end++) = digit += '0';
        else            *(end++) = digit += 'A' - 10;
    }
    if (round_up)
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
