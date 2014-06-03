#ifndef BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

#include <string>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>

namespace boost
{
    struct spirit_converter;
}

struct boost::spirit_converter
{
    bool operator()(std::string const& str, int& result) const
    {
        std::string::const_iterator i = str.begin();

        if (!boost::spirit::qi::parse(i, str.end(), boost::spirit::int_, result))
            return false;

        return i == str.end(); // ensure the whole string was parsed
    }
};

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
