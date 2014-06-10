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
    bool operator()(std::string const& str, boost::optional<int>& result) const
    {
        std::string::const_iterator i = str.begin();

        if (!boost::spirit::qi::parse(i, str.end(), boost::spirit::int_, *(result = int())))
            return false;

        return i == str.end(); // ensure the whole string was parsed
    }
};

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
