/* In this exercise, we will look back at input/output streams and
 * formatting operators. */

#include <sstream>
#include <cassert>

 /* Implement class ‹card› which represents one card from the
  * standard 52-card deck, along with operators for input and output.
  * The format is two letters, first the rank and then the suit.  The
  * rank 10 is represented as ‹T›. Use S, H, C and D to represent
  * suits. Do not forget to handle errors. */

struct card 
{
    char color; //SHCD
    char rank;  //A23456789TJQK
    card() = default;
};

std::ostream& operator<<(std::ostream& os, const card& c)
{
    os << c.rank << c.color;
    return os;
}

std::istream& operator>>(std::istream& is, card& c)
{
    card tmp;
    is >> tmp.rank >> tmp.color;
    const std::string r = "A23456789TJQK";
    const std::string col = "SHCD";
    if (r.find(tmp.rank) == std::string::npos || col.find(tmp.color) == std::string::npos) {
        is.setstate(std::ios::failbit);
    }
    else {
        c = tmp;
    }
    return is;
}

int main()
{
    std::istringstream s_in("AS 4D 5H 44");
    std::ostringstream s_out;

    card c, d;

    auto chkbad = [&](std::string s)
    {
        s_in.str(s);
        s_in.clear();
        s_out.str("");
        assert(!(s_in >> c));
        s_out << c;
        assert(s_out.str() == "4D");
    };

    auto chkgood = [&](std::string s)
    {
        s_in.str(s);
        s_in.clear();
        s_out.str("");
        assert(s_in >> c);
        s_out << c;
        assert(s_out.str() == s);
    };

    assert(s_in >> c);
    s_out << c;
    assert(s_out.str() == "AS");
    s_out.str("");
    assert(s_in >> c >> d);
    assert(!(s_in >> c));
    s_out << c << " " << d;
    assert(s_out.str() == "4D 5H");

    chkbad("AA");
    chkbad("A");
    chkbad("SA");
    chkbad("4");

    chkgood("AD");
}
