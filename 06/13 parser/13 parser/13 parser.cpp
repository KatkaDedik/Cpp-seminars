/* Implement a class ‹expr› (it will represent an abstract syntax
 * tree) and a function, ‹parse›, which reads data from an ‹istream›
 * and returns an instance of ‹expr›. Semantically, the parsed
 * language contains lists and atoms. Atoms are strings made of
 * alphanumeric characters, while lists start with a left
 * parenthesis, followed by 0 or more atoms or lists separated by
 * whitespace and close with a right parenthesis. */

 /* The ‹expr› class is used to represent both atoms and lists. To
  * distinguish which is which, it should have methods ‹is_list› and
  * ‹is_atom› which both return a ‹bool›. Additionally, there should
  * be methods ‹begin›, ‹end› and ‹atom›, all of which are ‹const›
  * (the structure is, after it is created by the parser, immutable).
  * The ‹atom› method should return an ‹std::string›. The ‹begin› and
  * ‹end› methods return iterators (i.e. suitable values that can be
  * incremented using ‹++› and dereferenced using unary ‹*›). Calling
  * ‹atom› on a list or ‹begin›/‹end› on an atom are undefined. */

class expr;

/* Finally, the parser itself. */

expr parse(std::istream &input);

int main()
{
	/* TBD */
}
