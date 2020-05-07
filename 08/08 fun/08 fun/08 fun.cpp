/* In this exercise, we will compute a few interesting higher-order
 * functions. The base functions (which will appear as arguments of
 * the higher-order functions) will work with integers and sets of
 * integers. */

#include <cassert>

 /* Given two functions ‹f› and ‹g›, construct a new function which
  * applies ‹g› to an input ‹int› to obtain ‹x›, then returns the
  * result of applying ‹g› to ‹x›. I.e. the returned function should
  * be the composition ⟦f ∘ g⟧. I.e.
  * ‹compose : (int → int) → (int → int) → int → int› */

  // static auto compose = []( auto f, auto g ) ... ref: 4 lines

  /* Given a function ‹f› that takes an ‹int› and returns an ‹int›,
   * construct a function which applies ‹f› to each element of an
   * input set and returns the resulting set. I.e.
   * ‹fmap : (int → int) → {int} → {int}› */

   // static auto fmap = []( auto f ) ... ref : 10 lines

   /* Implement ‹bind› which takes a function ‹f› which, given an
	* ‹int›, produces a set of integers. The result of ‹bind› is a new
	* function, which applies ‹f› to each element of an input set and
	* computes the union of all the results to create a single set of
	* integers. I.e. ‹bind : (int → {int}) → {int} → {int}›. */

	// static auto bind = []( auto f ) ... ref: 11 lines

int main()
{
	auto f = [](int x) { return x / 2; };
	auto g = [](int x) { return x * 2; };

	auto fg = compose(f, g);
	auto gf = compose(g, f);

	assert(fg(3) == 3);
	assert(gf(3) == 2);

	std::set x{ 1, 2, 3, 4, 5, 6 };

	assert(fmap(f)(x) == (std::set{ 0, 1, 2, 3 }));
	assert(fmap(g)(x) == (std::set{ 2, 4, 6, 8, 10, 12 }));
	assert(fmap(fg)(x) == x);
	assert(fmap(gf)(x) == (std::set{ 0, 2, 4, 6 }));

	auto h = [](int x)
	{
		std::set< int > o;
		for (int i = 0; i < 10; i += x)
			o.insert(i);
		return o;
	};

	assert(h(3) == (std::set{ 0, 3, 6, 9 }));
	assert(bind(h)(x) == (std::set{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

	std::set y{ 3, 4 };
	assert(bind(h)(y) == (std::set{ 0, 3, 4, 6, 8, 9 }));
}