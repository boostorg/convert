#include "./test.hpp"

using std::string;
using std::wstring;

static
bool
my_cypher(std::string const& value_in, boost::optional<std::string>& value_out)
{
    size_t const cypher = 'A' - '1';

    value_out = value_in;

    for (string::iterator it = value_out->begin(); it != value_out->end(); ++it)
        (*it < 'A') ? (*it += cypher) : (*it -= cypher);

    return true;
}

void
test::encryption()
{
    ////////////////////////////////////////////////////////////////////////////
    // Testing custom converter.
    ////////////////////////////////////////////////////////////////////////////

    string encrypted = boost::convert<string>("ABC", my_cypher).value();
    string decrypted = boost::convert<string>(encrypted, my_cypher).value();

    BOOST_TEST(encrypted == "123");
    BOOST_TEST(decrypted == "ABC");
}
