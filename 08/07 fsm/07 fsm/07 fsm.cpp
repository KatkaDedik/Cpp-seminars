/* Everyone's favourite: finite state machines. We will write a
 * class that will let us decide whether a word belongs to a regular
 * language described by a given finite automaton (finite state
 * machine) or not. */

#include <cassert>
#include <map>

 /* The constructor of ‹fsm› should accept a single boolean (mark the
  * constructor ‹explicit›), which determines if the state
  * represented by the instance is accepting or not. A
  * default-constructed ‹fsm› should be non-accepting. Make the
  * following methods available:
  *
  *  • ‹next› which accepts a letter (of type ‹char›) and a ‹const›
  *    reference to another ‹fsm› instance; the method adds a
  *    transition to the automaton,
  *  • ‹accept› which accepts a string and returns ‹true› if the word
  *    stored in the string is accepted by the automaton; this method
  *    should be marked ‹const›. */

class fsm {

	bool accepting;
	std::map<char, const fsm *> nodes;

public:
	explicit fsm(bool ac = false) : accepting(ac), nodes() {}

	void next(const char letter, const fsm & n)
	{
		nodes[letter] = &n;
	}

	bool isAccepting() const 
	{
		return accepting;
	}

	std::map<char, const fsm *> getNodes() const
	{
		return nodes;
	}

	bool accept(const std::string & str) const 
	{
		const fsm * current = this;
		for (char c : str) 
		{
			const std::map<char, const fsm *> & n = current->getNodes();

			if (n.find(c) != n.end()) 
			{
				current = n.at(c);
			}
			else 
			{
				return false;
			}
		}
		return current->isAccepting();
	}

}; /* ref: 17 lines */

int main()
{
	fsm a(true), b, c, d;

	assert(a.accept(""));
	assert(!a.accept("x"));
	assert(!b.accept(""));

	b.next('x', a);
	a.next('x', a);

	assert(a.accept(""));
	assert(a.accept("x"));
	assert(a.accept("xx"));
	assert(a.accept("xxx"));

	c.next('z', a);
	assert(c.accept("z"));
	assert(c.accept("zx"));

	d.next('y', d);
	d.next('z', c);

	assert(!d.accept(""));
	assert(!d.accept("y"));
	assert(!d.accept("yy"));
	assert(!d.accept("yyz"));
	assert(d.accept("yyzz"));
	assert(d.accept("yyzzx"));
	assert(!d.accept("yyzzzx"));
	assert(d.accept("yyzzxx"));
}
