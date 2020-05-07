/* In this exercise, we will implement a set of «arbitrary»
 * integers, with the following operations: union using ‹|›,
 * intersection using ‹&›, difference using ‹-› and inclusion using
 * ‹<=›. Use efficient algorithms for the operations (check out
 * what's available in the standard header ‹algorithm›). Provide
 * methods ‹add› and ‹has› to add elements and test their presence.
 */

#include <cassert>
#include <set>

class set {
	
	std::set<int> s;

public:

	explicit set(){}

	void add(int i){
		s.insert(i);
	}

	std::set<int> get() const {
		return s;
	}

	bool has(int number) const {
		return (s.find(number) != s.end());
	}

	friend set operator|(set a, set b) {
		set n;
		for (int p : a.get()){
			n.add(p);
		}
		for (int p : b.get()) {
			n.add(p);
		}
		return n;
	}

	friend set operator&(set a, set b) {
		set n;
		for (int p : a.get()) {
			if (b.has(p)) {
				n.add(p);
			}
		}
		return n;
	}

	friend set operator-(set a, set b) {
		set n;
		for(int p : a.get()) {
			if (!b.has(p)) {
				n.add(p);
			}
		}
		return n;
	}

	friend bool operator<=(set a, set b) {
		for (int p : a.get()) {
			if (!b.has(p)) {
				return false;
			}
		}
		return true;
	}
};

int main()
{
	set a, b;
	a.add(1), a.add(2), a.add(5), a.add(-3);
	b.add(1), b.add(-3), b.add(-1000), b.add(1 << 20);

	const set x = a, y = b;

	const set u = x | y;
	const set i = x & y;
	const set d = x - y;

	assert(u.has(1));
	assert(i.has(1));
	assert(!d.has(1));

	assert(u.has(2));
	assert(!i.has(2));
	assert(d.has(2));
	assert(!i.has(-1000));
	assert(u.has(-1000));
	assert(!d.has(-1000));
	assert(u.has(1 << 20));
	assert(!i.has(1 << 20));

	assert(!(a <= b));
	assert(!(b <= a));
	assert(a <= u);
	assert(b <= u);
	assert(i <= u);
	assert(i <= a);
	assert(i <= b);
	assert(u <= u);
	assert(d <= u);
	assert(d <= a);
	assert(!(d <= b));
}