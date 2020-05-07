/* Another exercise, another data structure. This time we will look
 * at so-called «unrolled linked lists». We will need the data
 * structure itself, with ‹begin›, ‹end›, ‹empty› and ‹push_back›
 * methods. As usual, we will store integers. The difference between
 * a 'normal' singly-linked list and an unrolled list is that in the
 * latter, each node stores more than one item. In this case, we
 * will use 4 items per node. Of course, the last node might only be
 * filled partially. The iterator that ‹begin› and ‹end› return
 * should at least implement dereference, pre-increment and
 * inequality, as usual. We will not provide an interface for
 * erasing elements, because that is somewhat tricky. */

#include <cassert>
#include <memory>


struct unrolled_node {
	std::unique_ptr< unrolled_node > next;
	int items [4];
	int freeSpace;

	unrolled_node(int number) : next(nullptr), items{ number, NULL, NULL, NULL }, freeSpace(3){};

	void add(int number) {
		if (freeSpace != 0) {
			items[4 - freeSpace] = number;
			freeSpace--;
		}
	}

	friend bool operator!=(unrolled_node first, unrolled_node second) {
		for (int i = 0; i < 4; i++) {
			if (first.items[i] != second.items[i]) {
				return false;
			}
		}
		return true;
	}
};     

struct unrolled_iterator {
	unrolled_node *node;
	int pos;

	unrolled_iterator(unrolled_node *n) : node(n), pos(0) {}

	unrolled_iterator &operator++()
    {
		pos++;
		if (pos == 4) {
			node = node->next.get();
			pos = 0;
		}
		else if (!node->items[pos]) 
		{
			node = node->next.get();
		}
        return *this;
     }

	bool operator!=(const unrolled_iterator &n) const {
		return n.node != node;
	}

	int &operator*() { return node->items[pos]; }
	int  operator*() const { return node->items[pos]; }

}; /* ref: 22 lines */

class unrolled {

	std::unique_ptr< unrolled_node > first;
	unrolled_node* last;

public:
	unrolled() :first(nullptr), last(nullptr) {};

	bool empty() { return !last; }

	unrolled_iterator begin() { return { first.get() }; }

	unrolled_iterator end() const { return { nullptr }; }

	void push_back(int v) 
	{
		if (last)
		{
			if (last->freeSpace == 0) {
				last->next.reset(new unrolled_node(v));
				last = last->next.get();
				return;
			}
			last->add(v);
		}
		else
		{
			first.reset(new unrolled_node(v));
			last = first.get();
		}
	}	
};

int main()
{
	/* Basic properties of an empty list. */
	unrolled u;
	assert(u.empty());
	assert(!(u.begin() != u.end()));

	/* Basic properties of a non-empty list. */

	u.push_back(3);
	assert(!u.empty());
	auto i = u.begin();

	assert(i != u.end());
	++i;
	assert(!(i != u.end()));

	/* Helper function to check the content of the list. */

	auto check = [&](int total)
	{
		int count = 0;
		for (int x : u) { assert(x == count++ + 3); }
		assert(count == total);
	};

	/* Add things to the list and check they are there. */

	check(1);

	for (int i = 0; i < 10; ++i)
	{
		u.push_back(4 + i);
		check(2 + i);
	}
}
