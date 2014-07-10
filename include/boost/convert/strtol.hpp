// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <climits>

namespace boost { namespace cnv
{
    struct strtol;
}}

/// @brief std::strtol-based extended converter
/// @details The converter offers a fairly decent overall performance and moderate formatting facilities.

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase<boost::cnv::strtol>
{
    // C1. Functions cannot be generalized to take [begin, end) ranges.
    //     Main functions have to take "char const*" as that is what strtol() and its family
    //     work with. That is, strtol() expects a \0 terminated string.
    //     So, feeding ranges is wrong as ranges are not mandated to be contiguous and
    //     to have the trailing \0. Therefore, strol() will fail.

    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    void str_to(char const* v, optional<   int_type>& r) const { strtol_  (v, r); } //C1
    void str_to(char const* v, optional<  sint_type>& r) const { strtol_  (v, r); }
    void str_to(char const* v, optional<  lint_type>& r) const { strtol_  (v, r); }
    void str_to(char const* v, optional<  uint_type>& r) const { strtoul_ (v, r); }
    void str_to(char const* v, optional< usint_type>& r) const { strtoul_ (v, r); }
    void str_to(char const* v, optional< ulint_type>& r) const { strtoul_ (v, r); }
    void str_to(char const* v, optional<   flt_type>& r) const { strtod_  (v, r); }
    void str_to(char const* v, optional<   dbl_type>& r) const { strtod_  (v, r); }
    void str_to(char const* v, optional<  ldbl_type>& r) const { strtod_  (v, r); }

    template<typename T> void str_to(std::string const& v, optional<T>& r) const
    {
        if (!v.empty()) str_to(v.c_str(/*C1*/), r);
    }

    detail::str_range to_str ( int_type v, char* buf) const { return i_to_str(v, buf); }
    detail::str_range to_str (lint_type v, char* buf) const { return i_to_str(v, buf); }
    detail::str_range to_str ( dbl_type v, char* buf) const;

    private:

    template<typename Type> void  strtod_ (char const*, optional<Type>&) const;
    template<typename Type> void  strtol_ (char const*, optional<Type>&) const;
    template<typename Type> void strtoul_ (char const*, optional<Type>&) const;

    template<typename T> detail::str_range i_to_str (T, char*) const;
    static double           adjust_fraction (double, int precision);
    static int                     get_char (int v) { return (v < 10) ? (v += '0') : (v += 'A' - 10); }
};

template<typename out_type>
void
boost::cnv::strtol::strtol_(char const* str, optional<out_type>& result_out) const
{
    if (!skipws_ && std::isspace(*str))
        return;

    char const*       str_end = str + strlen(str);
    char*             cnv_end = 0;
    llint_type const   result = std::strtoll(str, &cnv_end, base_);
    bool const           good = result != LLONG_MIN && result != LLONG_MAX && cnv_end == str_end;
    static out_type const min = std::numeric_limits<out_type>::min();
    static out_type const max = std::numeric_limits<out_type>::max();

    if (good && min <= result && result <= max)
        result_out = out_type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtoul_(char const* str, optional<Type>& result_out) const
{
    if (!skipws_ && std::isspace(*str))
        return;

    char const*      str_end = str + strlen(str);
    char*            cnv_end = 0;
    ullint_type const result = std::strtoull(str, &cnv_end, base_);
    bool const          good = result != ULLONG_MAX && cnv_end == str_end;
    Type const           max = std::numeric_limits<Type>::max();

    if (good && result <= max)
        result_out = Type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtod_(char const* str, optional<Type>& result_out) const
{
    if (!skipws_ && std::isspace(*str))
        return;

    char const*    str_end = str + strlen(str);
    char*          cnv_end = 0;
    ldbl_type const result = std::strtold(str, &cnv_end);
    bool const        good = result != -HUGE_VALL && result != HUGE_VALL && cnv_end == str_end;
    Type const         max = std::numeric_limits<Type>::max();

    if (good && -max <= result && result <= max)
        result_out = Type(result);
}

template<typename Type>
boost::cnv::detail::str_range
boost::cnv::strtol::i_to_str(Type value, char* buf) const
{
    // C1. Base=10 optimization improves performance 10%

    char*           beg = buf + bufsize_ / 2;
    char*           end = beg;
    bool const negative = (value < 0) ? (value = -value, true) : false;

    if (base_ == 10) for (; value; *(--beg) = int(value % 10) + '0', value /= 10); //C1
    else             for (; value; *(--beg) = get_char(value % base_), value /= base_);

    if (beg == end) *(--beg) = '0';
    if (negative)   *(--beg) = '-';

    return detail::str_range(beg, end);
}

inline
double
boost::cnv::strtol::adjust_fraction(double fraction, int precision)
{
    // C1. Bring forward the fraction coming right after precision digits.
    //     That is, say, fraction=0.234567, precision=2. Then brought forward=23.4567
    // C3. INT_MAX(4bytes)=2,147,483,647. So, 10^8 seems appropriate. If not, drop it down to 4.
    // C4. ::round() returns the integral value that is nearest to x,
    //     with halfway cases rounded away from zero. Therefore,
    //          round( 0.4) =  0
    //          round( 0.5) =  1
    //          round( 0.6) =  1
    //          round(-0.4) =  0
    //          round(-0.5) = -1
    //          round(-0.6) = -1

    int const tens[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

    for (int k = precision / 8; k; --k) fraction *= 100000000; //C3.

    fraction *= tens[precision % 8]; //C1

//  return ::rint(fraction); //C4
    return ::round(fraction); //C4
}

inline
boost::cnv::detail::str_range
boost::cnv::strtol::to_str(double value, char* buf) const
{
    char*           beg = buf + bufsize_ / 2;
    char*           end = beg;
    char*          ipos = end - 1;
    bool const negative = (value < 0) ? (value = -value, true) : false;
    double        ipart = std::floor(value);
    double        fpart = adjust_fraction(value - ipart, precision_);
    int       precision = precision_;
    int const      base = 10;

    for (; 1 <= ipart; ipart /= base)
        *(--beg) = get_char(int(ipart - std::floor(ipart / base) * base));

    if (beg == end) *(--beg) = '0';
    if (precision)  *(end++) = '.';

    for (char* fpos = end += precision; precision; --precision, fpart /= base)
        *(--fpos) = get_char(int(fpart - std::floor(fpart / base) * base));

    if (1 <= fpart)
    {
        for (; beg <= ipos; --ipos)
            if (*ipos == '9') *ipos = '0';
            else { ++*ipos; break; }

        if (ipos < beg)
            *(beg = ipos) = '1';
    }
    if (negative) *(--beg) = '-';

    return detail::str_range(beg, end);
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
