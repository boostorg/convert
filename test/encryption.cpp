#include "./test.hpp"

using std::string;
using std::wstring;
using boost::convert;

static
bool
my_cypher(std::string const& value_in, std::string& value_out)
{
    size_t const cypher = 'A' - '1';

    value_out = value_in;

    for (string::iterator it = value_out.begin(); it != value_out.end(); ++it)
        (*it < 'A') ? (*it += cypher) : (*it -= cypher);

    return true;
}

void
test::encryption()
{
    ////////////////////////////////////////////////////////////////////////////
    // Testing custom converter.
    ////////////////////////////////////////////////////////////////////////////

    string encrypted = convert<string>::from("ABC", my_cypher).value();
    string decrypted = convert<string>::from(encrypted, my_cypher).value();

    BOOST_TEST(encrypted == "123");
    BOOST_TEST(decrypted == "ABC");
}
