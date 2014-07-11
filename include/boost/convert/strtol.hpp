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
    namespace detail
    {
        template<typename T> struct types {};

        template<> struct types<int>
        {
            typedef unsigned int unsigned_type;
        };
        template<> struct types<short int>
        {
            typedef unsigned int unsigned_type;
        };
        template<> struct types<long int>
        {
            typedef unsigned long int unsigned_type;
        };
        template<> struct types<long long int>
        {
            typedef unsigned long long int unsigned_type;
        };
    };
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
    //     to have the trailing \0. Otherwise, strol() will fail.
    // C2. Old C-strings have an advantage over [begin, end) ranges. They do not need the 'end' iterator!
    //     Instead, they provide a sentinel (0 terminator). Consequently, C strings can be traversed
    //     without the need to compare if the 'end' has been reached (i.e. "for (; it != end; ++it)").
    //     Instead, the current character is checked if it's 0 (i.e. "for (; *p; ++p)") which is faster.
    //
    //     So, the implementation takes advantage of the fact. Namely, we simply check if *cnv_end == 0
    //     instead of traversing once with strlen() to find the end iterator and then comparing to it as in
    //
    //         char const* str_end = str + strlen(str); // Unnecessary traversal!
    //         ...
    //         bool const     good = ... && cnv_end == str_end;

    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    void str_to(char const* v, optional<   int_type>& r) const { strtoi_  (v, r); } //C1
    void str_to(char const* v, optional<  sint_type>& r) const { strtoi_  (v, r); }
    void str_to(char const* v, optional<  lint_type>& r) const { strtoi_  (v, r); }
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
    template<typename Type> void  strtoi_ (char const*, optional<Type>&) const;
    template<typename Type> void  strtou_ (char const*, optional<Type>&) const;
    template<typename Type> void strtoul_ (char const*, optional<Type>&) const;

    template<typename T> detail::str_range i_to_str (T, char*) const;
    static double           adjust_fraction (double, int precision);
    static int                     get_char (int v) { return (v < 10) ? (v += '0') : (v += 'A' - 10); }
};

template<typename Type>
void
boost::cnv::strtol::strtoul_(char const* str, optional<Type>& result_out) const
{
    if (!skipws_ && std::isspace(*str))
        return;

    char*            cnv_end = 0;
    ullint_type const result = std::strtoull(str, &cnv_end, base_);
    bool const          good = result != ULLONG_MAX && *cnv_end == 0/*C2*/;
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

    char*          cnv_end = 0;
    ldbl_type const result = std::strtold(str, &cnv_end);
    bool const        good = result != -HUGE_VALL && result != HUGE_VALL && *cnv_end == 0/*C2*/;
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

template<typename out_type>
void
boost::cnv::strtol::strtoi_(char const* s, boost::optional<out_type>& result_out) const
{
    typedef typename detail::types<out_type>::unsigned_type unsigned_type;

    /**/ if (skipws_) for (; std::isspace(*s); ++s);
    else if (std::isspace(*s)) return;

    unsigned int   base = base_;
    unsigned int     ch = *s++;
    bool const negative = ch == '-' ? (ch = *s++, true) : ch == '+' ? (ch = *s++, false) : false;

    if ((base == 0 || base == 16) && ch == '0' && (*s == 'x' || *s == 'X'))
    {
        ch = *++s; ++s; base = 16;
    }
    if (base == 0)
        base = ch == '0' ? 8 : 10;

    unsigned_type const    max = std::numeric_limits<out_type>::max() + (negative ? 1 : 0);
    unsigned_type const cutoff = max / base;
    unsigned int  const cutlim = max % base;
    unsigned_type       result = 0;

    for (; ch; ch = *s++)
    {
        /**/ if (std::isdigit(ch)) ch -= '0';
        else if (std::isalpha(ch)) ch -= (std::isupper(ch) ? 'A' : 'a') - 10;
        else return;

        if (base <= ch || cutoff < result || (result == cutoff && cutlim < ch))
            return;

        result *= base;
        result += ch;
    }
    result_out = negative ? -out_type(result) : out_type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtou_(char const* s, boost::optional<Type>& result_out) const
{
    /**/ if (skipws_) for (; std::isspace(*s); ++s);
    else if (std::isspace(*s)) return;

    unsigned int   base = base_;
    unsigned int     ch = *s++;

    if ((base == 0 || base == 16) && ch == '0' && (*s == 'x' || *s == 'X'))
    {
        ch = *++s; ++s; base = 16;
    }
    if (base == 0)
        base = ch == '0' ? 8 : 10;

    static Type const max = std::numeric_limits<Type>::max();
    Type           cutoff = max / base;
    unsigned int   cutlim = max % base;
    Type           result = 0;

    for (; ch; ch = *s++)
    {
        /**/ if (std::isdigit(ch)) ch -= '0';
        else if (std::isalpha(ch)) ch -= (std::isupper(ch) ? 'A' : 'a') - 10;
        else return;

        if (ch >= base)
            return;

        if (result > cutoff || (result == cutoff && ch > cutlim))
            return;

        result *= base;
        result += ch;
    }
    result_out = result;
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
