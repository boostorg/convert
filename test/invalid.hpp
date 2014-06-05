#ifndef BOOST_CONVERT_TEST_INVALID_HPP
#define BOOST_CONVERT_TEST_INVALID_HPP

#include "./test.hpp"

template<typename Converter>
void
test::invalid(Converter const& cnv)
{
    char const* str[] = { "not", "1 2", " 33", "44 ", "0x11", "7 + 5" };
    int const    size = sizeof(str) / sizeof(str[0]);

    for (int k = 0; k < size; ++k)
    {
        boost::cnv::optional<int> const res = boost::convert<int>(str[k], cnv);
        bool const                   failed = !res;

        if (!failed)
        {
            printf("test::invalid() failed for: <%s><%d>\n", str[k], res.value());
        }
        BOOST_TEST(failed);
    }
}

#endif // BOOST_CONVERT_TEST_INVALID_HPP
