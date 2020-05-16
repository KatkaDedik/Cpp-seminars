/* Ternary (or 3-valued) logic uses 3 different truth values: true,
 * false and unknown (maybe). */

#include <cassert>

 /* Define a suitable type ‹tristate› and 3 constants ‹yes›, ‹no› and
  * ‹maybe› (to avoid conflicts with built-in boolean constants),
  * along with the standard logical operators and equality. */

enum tristate {
    no = 0,
    yes = 1,
    maybe = -1
};

enum::tristate operator&&(enum::tristate first, enum::tristate second) {
    if (first == no || second == no) {
        return tristate::no;
    }
    if (first == yes && second == yes) {
        return tristate::yes;
    }
    return tristate::maybe;
}

enum::tristate operator||(enum::tristate first, enum::tristate second) {
    if (first == no && second == no) {
        return tristate::no;
    }
    if (first == yes || second == yes) {
        return tristate::yes;
    }
    return tristate::maybe;
}

int main()
{
    assert((yes && no) == no);
    assert((no && yes) == no);
    assert((no && no) == no);
    assert((yes && yes) == yes);

    assert(((yes && no) && maybe) == no);
    assert(((yes && no) || maybe) == maybe);
    assert(((yes && no) || maybe) == maybe);

    assert((yes && yes && maybe) == maybe);
    assert((yes && maybe && yes) == maybe);
    assert((maybe && yes && yes) == maybe);

    assert((no || maybe || yes) == yes);
    assert((no || yes || maybe) == yes);
    assert((no || maybe || no) == maybe);
}
