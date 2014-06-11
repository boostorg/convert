#ifndef BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

#include <string>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>

namespace boost { namespace cnv
{
    struct spirit;
}}

struct boost::cnv::spirit
{
    template<typename Iterator>
    void parse_(Iterator beg, Iterator end, boost::optional<int>& result) const
    {
        if (boost::spirit::qi::parse(beg, end, boost::spirit::int_, *(result = int())))
            if (beg == end) // ensure the whole string was parsed
                return;

        result.reset();
    }
    void operator()(std::string const& str, boost::optional<int>& result) const
    {
        parse_(str.begin(), str.end(), result);
    }
    void operator()(char const* str, boost::optional<int>& result) const
    {
        parse_(str, str + strlen(str), result);
    }
};

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

